#include <stdio.h>

#include "gtthread.h"

/* Test Case #1
 *  * 1. Create num_threads thread1() in main().
 *  * 2. main() join all threads created in step 1
 *  * 3. main() print all returned values from threads
 *  *
 *  * Tested functions:
 *  * 1. gtthread_init(1)
 *  * 2. gtthread_create(3)
 *  * 3. gtthread_join(2)
 *  */

const int num_threads = 10;
const int period = 20;
void *thread1(void* a)
{
      int i = 0;
      int v = *(int*)a;
      int *p = (int*)malloc(sizeof(int));
      printf("[thread %d] started\n", v);
      while (i < 100000000) {
          if (i % 20000000 == 0) 
              printf("[thread%d] is still running\n", v);
          ++i;
      }
      *p = v;
      printf("[thread %d] finished, *p is %d\n", v, *p);
      return (void *)p;
}

int main()
{
      int i, o[num_threads];
      int a;
      int *rt[num_threads];
      for (i=0; i<num_threads; i++) {
          rt[i] = (int *)malloc(sizeof(int));
          o[i] = i;
      }
      puts("[main] started");
      gtthread_init(period);
      gtthread_t th[num_threads];

      for (i=0; i<num_threads; ++i) {
          gtthread_create(&th[i], thread1, (void*)&o[i]);
      }
      for (i=0; i<num_threads; ++i) {
          gtthread_join(th[i], (void*)&rt[i]);
      }
      for (i=0; i<num_threads; ++i)
          printf("[main] rt[%d]: %d\n", i, *rt[i]);
      puts("[main] finished");
      return 0;
}
