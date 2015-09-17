// Test5
// gtthread_equal.

#include <stdio.h>
#include <gtthread.h>

void* worker(void* arg)
{
	int i;
	for(i = 0; i < 99999999; ++i);
}

int main()
{
	gtthread_t th1, th2;

	gtthread_init(1000);

	gtthread_create(&th1, worker, (void*)1);
	gtthread_create(&th2, worker, (void*)1);

	if(gtthread_equal(th1, th2)) {
		fprintf(stderr, 
				"!ERROR! They are different! %p, %p\n",
				th1, th2);
	}

	gtthread_join(th1, NULL);
	gtthread_join(th2, NULL);
	return 0;
}
