// Test6
// gtthread_self. The program should print out liek below.
//
// same or diff
// diff    same

#include <stdio.h>
#include <gtthread.h>

gtthread_t g_th1, g_th2;

void* worker(void* arg)
{
	gtthread_t tid = gtthread_self();

	if(gtthread_equal(g_th2, tid))
		printf("same\n");
	else
		printf("diff\n");
}

int main()
{
	int rc;

	gtthread_init(1000);

	rc = gtthread_create(&g_th1, worker, NULL);
	rc = gtthread_create(&g_th2, worker, NULL);

	gtthread_join(g_th1, NULL);
	gtthread_join(g_th2, NULL);
	return 0;
}
