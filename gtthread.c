/*
 * gtthread.c
 * gtthread
 * Created by Ning Wang on 9/8/15.
 * Copyright (c) 2015 Ning Wang. All rights reserved.
 */

#include "gtthread.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define UNDEF 0 /* undefined */
#define EX 0 /* the thread is executed */
#define ST 1 /* the thread is stopped */

typedef void handler_t(int);

// global data
static int maxtid;
static queue_t* ready_queue;
static gtthread_t* current;
static struct itimerval timer;

// private functions
handler_t *Signal(int signum, handler_t *handler);
void sigvtalrm_handler(int sig);
void preemptive_scheduler();



void gtthread_init(long period)
{
    maxtid = 0;
    current = thread_null;
    
    //initializing data structures
    ready_queue = (queue_t*) malloc(sizeof(queue_t));
    ready_queue->front = thread_null;
    ready_queue->back = thread_null;
    
    // set alarm signal and signal handler
    timer.it_interval = period;
    timer.it_value = period;
    setitimer(ITIMER_VIRTUAL, &timer, NULL);
    Signal(SIGVTALRM, sigvtalrm_handler);
}

int gtthread_create(gtthread_t *thread,
                    void *(*start_routine)(void *),
                    void *arg)
{
    ucontext_t* ucp;
    int state = ST;  
    ucp = (ucontext_t*) malloc(sizeof(ucontext_t)); 
    thread->tid = maxtid++;
    if (getcontext(ucp))
    {
        // enter here from setcontext
    }
    makecontext(ucp, start_routine, 1, arg);
    
    enqueue(ready_queue, thread);
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
    
}

void preemptive_scheduler()
{
    
}
