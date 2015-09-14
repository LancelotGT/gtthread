#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

  gtthread_init(1000);
  int i = 0;
  gtthread_create( &t1, thr1, NULL);
  while(i++ < 10000000);
  gtthread_yield();

  return EXIT_SUCCESS;
}
