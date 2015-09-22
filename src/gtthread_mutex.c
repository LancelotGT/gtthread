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
    steque_init(mutex);
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);
    return 0;
}

/*
  The gtthread_mutex_lock() is analogous to pthread_mutex_lock.
  Returns zero on success.
 */
int gtthread_mutex_lock(gtthread_mutex_t* mutex){
    sigprocmask(SIG_BLOCK, &vtalrm, NULL); 

    /* if queue lock is empty */
    if (steque_isempty(mutex))
    {
        steque_enqueue(mutex, (steque_item) gtthread_self());  
        sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);   
        return 0;
    }

    /* if a thread try to do recursive lock */ 
    if (gtthread_self() == (gtthread_t) steque_front(mutex))
    {
        sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);
        return -1;
    }

    steque_enqueue(mutex, (steque_item) gtthread_self()); 
    while (gtthread_self() != (gtthread_t) steque_front(mutex)) 
    {
        sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); 
        /* the alarm signal should be delivered here */
        // sigvtalrm_handler(SIGVTALRM);
        int i = 0;
        while (i++ < 10000);
        sigprocmask(SIG_BLOCK, &vtalrm, NULL);
    }
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);  
    return 0; 
}

/*
  The gtthread_mutex_unlock() is analogous to pthread_mutex_unlock.
  Returns zero on success.
 */
int gtthread_mutex_unlock(gtthread_mutex_t *mutex){
    sigprocmask(SIG_BLOCK, &vtalrm, NULL);
    if (steque_isempty(mutex))
    {
        sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);
        return -1;
    }

    if ((gtthread_t) steque_front(mutex) != gtthread_self())
    {
       sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);
       return -1;
    }

    steque_pop(mutex);
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); 
    return 0; 
}

/*
  The gtthread_mutex_destroy() function is analogous to
  pthread_mutex_destroy and frees any resourcs associated with the mutex.
*/
int gtthread_mutex_destroy(gtthread_mutex_t *mutex){
    sigprocmask(SIG_BLOCK, &vtalrm, NULL); /* in case this is blocked previously */    
    steque_destroy(mutex);
    sigprocmask(SIG_UNBLOCK, &vtalrm, NULL); 
    return 0; 
}
