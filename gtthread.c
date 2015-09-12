/*
 * gtthread.c
 * gtthread
 *
 * Created by Ning Wang on 9/8/15.
 * Copyright (c) 2015 Ning Wang. All rights reserved.
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

typedef void handler_t(int);

/* global data section */
static int maxtid;
static queue_t ready_queue;
static gtthread_t* current;
static struct itimerval timer;

/* private functions prototypes */
handler_t *Signal(int signum, handler_t *handler);
void sigvtalrm_handler(int sig);
void preemptive_scheduler();


/* thread init routine, must be called before other routines */
void gtthread_init(long period)
{
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

int gtthread_create(gtthread_t *thread,
                    void *(*start_routine)(void *),
                    void *arg)
{
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

int gtthread_join(gtthread_t thread, void **status)
{
    return 0;
}

void gtthread_exit(void *retval)
{
    
}

int gtthread_yield(void)
{
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

int gtthread_equal(gtthread_t t1, gtthread_t t2)
{
    if (t1.tid == t2.tid)
        return 1;
    return 0;
}

int gtthread_cancel(gtthread_t thread)
{
    return 0;
}

gtthread_t gtthread_self(void)
{
    return *current;
}

int gtthread_mutex_init(gtthread_mutex_t *mutex)
{
    return 0;
}

int gtthread_mutex_lock(gtthread_mutex_t *mutex)
{
    return 0;
}

int gtthread_mutex_unlock(gtthread_mutex_t *mutex)
{
    return 0;
}

/* helper function to install the signal handler */
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
