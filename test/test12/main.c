// Test12
// two threads joining each other
// it should produce joining error

#include <stdio.h>
#include <gtthread.h>

gtthread_t th1;
gtthread_t th2;

void* worker1(void* arg)
{
  if (gtthread_join(th2, NULL) == -1)
      printf("Joining error.\n");
}

void* worker2(void* arg)
{
	int i;
	for(i = 0; i < 99999999; ++i);
  if (gtthread_join(th1, NULL) == -1)
      printf("Joining error.\n");
}

int main()
{
	gtthread_init(1000);

  gtthread_create(&th2, worker2, (void*)2); 
 	gtthread_create(&th1, worker1, (void*)1); 

	gtthread_join(th1, NULL);
	return 0;
}
