//
//  datastructure.c
//  gtthread
//
//  Created by Ning Wang on 9/8/15.
//  Copyright (c) 2015 Ning Wang. All rights reserved.
//

#include "datastructure.h"
#include <stdio.h>

void enqueue(queue_t* q, gtthread_t* t)
{
    t->next = thread_null;
    if (q->front == thread_null) {
        q->front = t;
        q->back = t;
    } else {
        q->back->next = t;
        q->back = t;
    }
}

gtthread_t* dequeue(queue_t* q)
{
    if (q->front == thread_null)
        return thread_null;
    if (q->front == q->back)
    {
        gtthread_t* temp = q->front;
        q->front = thread_null;
        q->back = thread_null;
        return temp;
    }
    else
    {
        gtthread_t* temp = q->front;
        q->front = q->front->next;
        return temp;
    }
}