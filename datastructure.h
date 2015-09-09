/*
 *  datastructure.h
 *  gtthread
 *
 *  Created by Ning Wang on 9/8/15.
 *  Copyright (c) 2015 Ning Wang. All rights reserved.
 */

#ifndef gtthread_datastructure_h
#define gtthread_datastructure_h

#include <ucontext.h>
#define thread_null (gtthread_t*) NULL

typedef struct GTThread_t
{
    struct GTThread_t* next;
    int tid;
    ucontext_t* context;
    int state;
} gtthread_t;

typedef struct GTThread_mutex_t
{
    
} gtthread_mutex_t;

typedef struct Queue_t {
    gtthread_t* front;
    gtthread_t* back;
} queue_t;

void enqueue(queue_t* q, gtthread_t* t);
gtthread_t* dequeue(queue_t* q);

#endif
