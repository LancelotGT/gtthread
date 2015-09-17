// Test9
// Mutex and gtthread_yield. The program should print like below.
//
// thread 1
// thread 1
// thread 1
// thread 2
// thread 2
// thread 2

#include <stdio.h>
#include <gtthread.h>

gtthread_mutex_t g_mutex;

void* worker(void* arg)
{
	int i;

	gtthread_mutex_lock(&g_mutex);
	for(i=0; i < 3; i++)
	{
		printf("thread %ld\n", (long)arg);
		gtthread_yield();
	}
	gtthread_mutex_unlock(&g_mutex);
}

int main()
{
	gtthread_t th1, th2;

	gtthread_init(1000);

	gtthread_mutex_init(&g_mutex);

	gtthread_create(&th1, worker, (void*)1);
	gtthread_create(&th2, worker, (void*)2);

	gtthread_join(th1, NULL);
	gtthread_join(th2, NULL);
	return 0;
}
