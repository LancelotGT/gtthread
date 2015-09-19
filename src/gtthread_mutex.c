/**********************************************************************
gtthread_mutex.c.  

This file contains the implementation of the mutex subset of the
gtthreads library.  The locks can be implemented with a simple queue.
 **********************************************************************/

/*
  Include as needed
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gtthread.h"
#define LOCK 1
#define UNLOCK 0

extern sigset_t vtalrm;

/*
  The gtthread_mutex_init() function is analogous to
  pthread_mutex_init with the default parameters enforced.
  There is no need to create a static initializer analogous to
  PTHREAD_MUTEX_INITIALIZER.
 */
int gtthread_mutex_init(gtthread_mutex_t* mutex)
{
    sigprocmask(SIG_BLOCK, &vtalrm, NULL); /* in case this is blocked previously */    
    mutex->tid = -1;
    mutex->lock = UNLOCK;
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);
    return 0;
}

/*
  The gtthread_mutex_lock() is analogous to pthread_mutex_lock.
  Returns zero on success.
 */
int gtthread_mutex_lock(gtthread_mutex_t* mutex){
    /* if a thread try to do recursive lock */
    sigprocmask(SIG_BLOCK, &vtalrm, NULL); 
    if (gtthread_self() == mutex->tid)
    {
        sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);
        return -1;
    }

    while(mutex->lock == LOCK)
    {
        sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); 
        /* the alarm signal should be delivered here */
        sigvtalrm_handler(SIGVTALRM);
        sigprocmask(SIG_BLOCK, &vtalrm, NULL);
    }
    mutex->lock = LOCK;
    mutex->tid = gtthread_self();
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);  
    return 0; 
}

/*
  The gtthread_mutex_unlock() is analogous to pthread_mutex_unlock.
  Returns zero on success.
 */
int gtthread_mutex_unlock(gtthread_mutex_t *mutex){
    sigprocmask(SIG_BLOCK, &vtalrm, NULL);
    if (mutex->lock == UNLOCK)
    {
        sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);
        return -1;
    }

    if (mutex->tid != gtthread_self())
    {
       sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);
       return -1;
    }

    mutex->lock = UNLOCK;
    mutex->tid = -1;
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); 
    return 0; 
}

/*
  The gtthread_mutex_destroy() function is analogous to
  pthread_mutex_destroy and frees any resourcs associated with the mutex.
*/
int gtthread_mutex_destroy(gtthread_mutex_t *mutex){
     return 1; 
}
