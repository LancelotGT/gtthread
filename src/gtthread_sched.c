/**********************************************************************
gtthread_sched.c.  

This file contains the implementation of the scheduling subset of the
gtthreads library.  A simple round-robin queue should be used.
 **********************************************************************/
/*
  Include as needed
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include "gtthread.h"
#include "steque.h"

#define GTTHREAD_RUNNING 0 /* the thread is running */
#define GTTHREAD_CANCEL 1 /* the thread is cancelled */
#define GTTHREAD_DONE 2 /* the thread has done */
#define GTTHREAD_WAITING 3 /* the thread is on the ready queue */

/* global data section */
static steque_t ready_queue;
static gtthread_t* current;
static struct itimerval timer;
static sigset_t vtalrm;
static int maxtid; 

/* private functions prototypes */
void sigvtalrm_handler(int sig);
void gtthread_start(void* (*start_routine)(void*), void* args);

/*
  The gtthread_init() function does not have a corresponding pthread equivalent.
  It must be called from the main thread before any other GTThreads
  functions are called. It allows the caller to specify the scheduling
  period (quantum in micro second), and may also perform any other
  necessary initialization.

  Recall that the initial thread of the program (i.e. the one running
  main() ) is a thread like any other. It should have a
  gtthread_t that clients can retrieve by calling gtthread_self()
  from the initial thread, and they should be able to specify it as an
  argument to other GTThreads functions. The only difference in the
  initial thread is how it behaves when it executes a return
  instruction. You can find details on this difference in the man page
  for pthread_create.
 */
void gtthread_init(long period){

    struct sigaction act;

    /* initializing data structures */
    maxtid = 0;
    steque_init(&ready_queue);
    
    /* create main thread and add it to ready queue */  
    gtthread_t* main_thread = (gtthread_t*) malloc(sizeof(gtthread_t));
    main_thread->tid = maxtid++;
    main_thread->ucp = (ucontext_t*) malloc(sizeof(ucontext_t)); 
    main_thread->arg = NULL;
    main_thread->state = GTTHREAD_RUNNING;

    /* must be called before makecontext */
    if (getcontext(main_thread->ucp) == -1)
    {
      perror("getcontext");
      exit(EXIT_FAILURE);
    }

    current = main_thread;
    // steque_enqueue(&ready_queue, main_thread); 
    
    /* setting uo the signal mask */
    sigemptyset(&vtalrm);
    sigaddset(&vtalrm, SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); /* in case this is blocked previously */

    /* set alarm signal and signal handler */
    timer.it_interval.tv_usec = 0;
    timer.it_interval.tv_sec = 0; 
    timer.it_value.tv_usec = period;
    timer.it_value.tv_usec = 0; 
    setitimer(ITIMER_VIRTUAL, &timer, NULL);
 
    /* install signal handler for SIGVTALRM */  
    memset(&act, '\0', sizeof(act));
    act.sa_handler = &sigvtalrm_handler;
    if (sigaction(SIGVTALRM, &act, NULL) < 0)
    {
      perror ("sigaction");
      exit(EXIT_FAILURE);
    }
}


/*
  The gtthread_create() function mirrors the pthread_create() function,
  only default attributes are always assumed.
 */
int gtthread_create(gtthread_t *thread,
		    void *(*start_routine)(void *),
		    void *arg){
    /* block SIGVTALRM signal */
    sigprocmask(SIG_BLOCK, &vtalrm, NULL);
    
   // ucontext_t* ucp;
   // ucp = (ucontext_t*) malloc(sizeof(ucontext_t)); 
    thread->tid = maxtid++; // need to block signal
    thread->state = GTTHREAD_WAITING;
    thread->proc = start_routine;
    thread->arg = arg;
    thread->ucp = (ucontext_t*) malloc(sizeof(ucontext_t));

    if (getcontext(thread->ucp) == -1)
    {
      perror("getcontext");
      exit(EXIT_FAILURE);
    }
    
    /* allocate stack for the newly created context */
    /* here we allocate the stack size using the canonical */
    /* size for signal stack. */
    thread->ucp->uc_stack.ss_sp = malloc(SIGSTKSZ);
    thread->ucp->uc_stack.ss_size = SIGSTKSZ;
    thread->ucp->uc_stack.ss_flags = 0;
    thread->ucp->uc_link = NULL;

    makecontext(thread->ucp, (void (*)(void)) gtthread_start, 2, start_routine, arg);
    steque_enqueue(&ready_queue, thread);

    /* unblock the signal */
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);   
    return 0; 
}

/*
  The gtthread_join() function is analogous to pthread_join.
  All gtthreads are joinable.
 */
int gtthread_join(gtthread_t thread, void **status){
    return 1;
}

/*
  The gtthread_exit() function is analogous to pthread_exit.
 */
void gtthread_exit(void* retval){
    /* block alarm signal */
    sigprocmask(SIG_BLOCK, &vtalrm, NULL);

    gtthread_t* prev = current; 
    gtthread_t* current = (gtthread_t*) steque_pop(&ready_queue);
    /* if no more thread to run, call exit(0) */ 
    if (!current)
        exit(0);  

    current->state = GTTHREAD_RUNNING; 

    /* free up memory allocated for current thread */
    free(prev->ucp->uc_stack.ss_sp); 
    free(prev->ucp);                
    prev->ucp = NULL;

    /* unblock alarm signal and setcontext for next thread */
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); 
    setcontext(current->ucp);
}


/*
  The gtthread_yield() function is analogous to pthread_yield, causing
  the calling thread to relinquish the cpu and place itself at the
  back of the schedule queue.
 */
int gtthread_yield(void){
    /* block SIGVTALRM signal */
    sigprocmask(SIG_BLOCK, &vtalrm, NULL);
    
    gtthread_t* next;
    if ((next = (gtthread_t*) steque_pop(&ready_queue)) == NULL)
      return 0;
    
    gtthread_t* prev = current;
    steque_enqueue(&ready_queue, current);
    current->state = GTTHREAD_WAITING;
    next->state = GTTHREAD_RUNNING; 
    current = next;

    /* unblock the signal */
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); 
    swapcontext(prev->ucp, current->ucp); 
    return 0; 
}

/*
  The gtthread_yield() function is analogous to pthread_equal,
  returning zero if the threads are the same and non-zero otherwise.
 */
int  gtthread_equal(gtthread_t t1, gtthread_t t2){
    return t1.tid == t2.tid;
}

/*
  The gtthread_cancel() function is analogous to pthread_cancel,
  allowing one thread to terminate another asynchronously.
 */
int  gtthread_cancel(gtthread_t thread){
    return 0;
}

/*
  Returns calling thread.
 */
gtthread_t gtthread_self(void){
    return *current;
}


/*
 * helper functions to install the signal handler 
 */

/* 
 * A wrapper function to start a routine.
 * The reason we need this is because we need to call gtthread_exit
 * when the thread finish executing.
 */
void gtthread_start(void* (*start_routine)(void*), void* args)
{
    /* unblock signal comes from gtthread_create */
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);

    /* start executing the start routine*/
    void* ret = (*start_routine)(args);

    /* when start_rountine returns, call gtthread_exit*/
    gtthread_exit(ret);
}

/*
 * A signal handler for SIGVTALRM
 * Comes here when a thread runs up its time slot. This handler implements
 * a preemptive thread scheduler. It looks at the global ready queue, pop
 * the thread in the front, save the current thread context and switch context. 
 */
void sigvtalrm_handler(int sig)
{
    /* block the signal */
    sigprocmask(SIG_BLOCK, &vtalrm, NULL);

    /* get the next runnable thread */
    gtthread_t* next = (gtthread_t*) steque_pop(&ready_queue);

    /* if only 1 thread in the queue, continue execution */
    if (next == NULL)
        return;

    gtthread_t* prev = current;
    steque_enqueue(&ready_queue, current);
    next->state = GTTHREAD_RUNNING; 
    current = next;

    /* unblock the signal */
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); 
    swapcontext(prev->ucp, current->ucp);
}

