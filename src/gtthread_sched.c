/**********************************************************************
gtthread_sched.c.  

This file contains the implementation of the scheduling subset of the
gtthreads library.  A simple round-robin queue should be used.
 **********************************************************************/
/*
  Include as needed
*/

#include "gtthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
 
#define GTTHREAD_RUNNING 0 /* the thread is running */
#define GTTHREAD_CANCEL 1 /* the thread is cancelled */
#define GTTHREAD_DONE 2 /* the thread has done */
#define GTTHREAD_WAITING 3 /* the thread is on the ready queue */

#define thread_null (gtthread_t*) NULL 
typedef void handler_t(int);

/* definition of gtthread */
typedef struct GTThread_t
{
    struct GTThread_t* next;
    int tid;
    ucontext_t* context;
    int state;
} gtthread_t; 

/* global data section */
static int maxtid;
static queue_t ready_queue;
static gtthread_t* current;
static struct itimerval timer;

/* private functions prototypes */
handler_t *Signal(int signum, handler_t *handler);
void sigvtalrm_handler(int sig);
void preemptive_scheduler();

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
   // /* block the alarm signal */
   // sigset_t mask_one;
   // sigemptyset(&mask_one);
   // sigaddset(&mask_one, SIGVTALRM);
   // sigprocmask(SIG_BLOCK, &mask_one, NULL);

    /* initializing data structures */
    maxtid = 0;
    ready_queue.front = thread_null;
    ready_queue.back = thread_null;
    
    /* set alarm signal and signal handler */
    timer.it_interval.tv_usec = 0;
    timer.it_interval.tv_sec = 0; 
    timer.it_value.tv_usec = period;
    timer.it_value.tv_usec = 0; 
    setitimer(ITIMER_VIRTUAL, &timer, NULL);

    /* create main thread and add it to ready queue */  
    gtthread_t* main_thread = (gtthread_t*) malloc(sizeof(gtthread_t));
    main_thread->tid = maxtid++;
    ucontext_t* ucp;
    ucp = (ucontext_t*) malloc(sizeof(ucontext_t)); 
    getcontext(ucp);

    current = main_thread;
    enqueue(&ready_queue, thread); 
    
    /* unblock the signal */
    //sigprocmask(sig_unblock, &mask_one, null);
    Signal(SIGVTALRM, sigvtalrm_handler); /* install signal handler for SIGVTALRM */ 
}


/*
  The gtthread_create() function mirrors the pthread_create() function,
  only default attributes are always assumed.
 */
int gtthread_create(gtthread_t *thread,
		    void *(*start_routine)(void *),
		    void *arg){
    /* block SIGVTALRM signal */
    sigset_t mask_one;
    sigemptyset(&mask_one);
    sigaddset(&mask_one, SIGVTALRM);
    sigprocmask(SIG_BLOCK, &mask_one, NULL);
    
    ucontext_t* ucp;
    ucp = (ucontext_t*) malloc(sizeof(ucontext_t)); 
    thread->tid = maxtid++; // need to block signal
    thread->state = GTTHREAD_WAITING;
    thread->proc = start_routine;
    thread->arg = arg;

    if (getcontext(ucp))
    {
      // get here from context switch from another thread
      printf("Warning: control should never reach here.");
      (*current->proc)(current->arg);
      gtthread_exit((void*));
    }
    
    /* allocate stack for the newly created context */
    /* here we allocate the stack size using the canonical */
    /* size for signal stack. */
    thread->ucp->uc_stack.ss_sp = malloc(SIGSTKSZ);
    thread->ucp->uc_stack.ss_size = SIGSTKSZ;
    thread->ucp->uc_stack.ss_flags = 0;

    makecontext(ucp, start_routine, 1, arg);
    enqueue(&ready_queue, thread);

    /* unblock the signal */
    sigprocmask(sig_unblock, &mask_one, null);   
    return 0; 
}

/*
  The gtthread_join() function is analogous to pthread_join.
  All gtthreads are joinable.
 */
int gtthread_join(gtthread_t thread, void **status){


}

/*
  The gtthread_exit() function is analogous to pthread_exit.
 */
void gtthread_exit(void* retval){



}


/*
  The gtthread_yield() function is analogous to pthread_yield, causing
  the calling thread to relinquish the cpu and place itself at the
  back of the schedule queue.
 */
void gtthread_yield(void){
    /* block SIGVTALRM signal */
    sigset_t mask_one;
    sigemptyset(&mask_one);
    sigaddset(&mask_one, SIGVTALRM);
    sigprocmask(SIG_BLOCK, &mask_one, NULL);
       
    if (current->next = thread_null)
      return 0;
    
    /* unblock the signal */
    sigprocmask(sig_unblock, &mask_one, null);    
    return 0; 
}

/*
  The gtthread_yield() function is analogous to pthread_equal,
  returning zero if the threads are the same and non-zero otherwise.
 */
int  gtthread_equal(gtthread_t t1, gtthread_t t2){


}

/*
  The gtthread_cancel() function is analogous to pthread_cancel,
  allowing one thread to terminate another asynchronously.
 */
int  gtthread_cancel(gtthread_t thread){

}

/*
  Returns calling thread.
 */
gtthread_t gtthread_self(void){

}


/*
 * helper function to install the signal handler 
 */
handler_t *Signal(int signum, handler_t *handler)
{
    struct sigaction action, old_action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask); /* block sigs of type being handled */
    action.sa_flags = SA_RESTART; /* restart syscalls if possible */

    if (sigaction(signum, &action, &old_action) < 0)
    {
        printf("unix error.\n");
        return (old_action.sa_handler);
    }
}

void sigvtalrm_handler(int sig)
{
    // scheduler should be called here and make context switch   
    preemptive_scheduler();
}

void preemptive_scheduler()
{
    // achieve preemptive scheduling
    // it looks the threads in ready queue, dequeue a thread and switch context
    gtthread* thread = dequeue(&ready_queue);
    swapcontext(current->ucp, thread->ucp);
    thread->state = EX;
    enqueue(current); 
    current = thread;
    makecontext(current->ucp, current->proc, 1, current->arg);
}

int alarmOn()
{
    /* block SIGVTALRM signal */
    sigset_t mask_one;
    sigemptyset(&mask_one);
    sigaddset(&mask_one, SIGVTALRM);
    sigprocmask(SIG_BLOCK, &mask_one, NULL); 
}

int alarmOff()
{
    /* unblock the signal */
    sigset_t mask_one;
    sigemptyset(&mask_one);
    sigaddset(&mask_one, SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &mask_one, NULL);  
}
