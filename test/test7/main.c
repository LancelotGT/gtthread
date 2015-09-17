// Test7
// gtthread_cancel. The program should terminate smoothly.

#include <stdio.h>
#include <gtthread.h>

void* worker2(void* arg)
{
	while(1);
}

void* worker(void* arg)
{
	gtthread_t th;

	gtthread_create(&th, worker2, NULL);
	gtthread_cancel(th);
	gtthread_join(th, NULL);

	while(1);
}

int main()
{
	gtthread_t th;

	gtthread_init(1000);
	gtthread_create(&th, worker, NULL);
	gtthread_cancel(th);
	gtthread_join(th, NULL);
	return 0;
}
