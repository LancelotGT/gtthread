#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "gtthread.h"

/* Tests creation.
   Should print "Hello World!" */

void *thr1(void *in) {
    printf("Hello World!\n");
    fflush(stdout);
    return NULL;
}

int main() {
    gtthread_t t1;

    gtthread_init(50000L);
    gtthread_create( &t1, thr1, NULL);

    while(1);
    
    return EXIT_SUCCESS;
}
