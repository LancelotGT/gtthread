// Test3
// Main exit. Program must terminate smoothly.

#include <stdio.h>
#include <gtthread.h>

void* worker(void* arg)
{
  int i;
	for(i = 0; i < 100000; i++);
}

int main()
{
	gtthread_t th1;

	gtthread_init(1000);

	gtthread_create(&th1, worker, NULL);

	printf("main exit\n");

	gtthread_exit(NULL);
	return 0;
}
