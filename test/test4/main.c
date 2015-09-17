// Test4
// Thread create chaining.
// The program should print out liek below when CHAIN_LEN is 20.
//
// worker1 starts
// worker2 starts
// ...
// worker20 starts
// worker20 ends
// worker19 ends
// ...
// worker 1 ends

#include <stdio.h>
#include <gtthread.h>

#define CHAIN_LEN 20

void* worker(void* arg)
{
	gtthread_t th;

	long id = (long) arg;
	printf("worker%ld starts\n", id);
	if (id < CHAIN_LEN) {
		gtthread_create(&th, worker, (void*) (id + 1));
		gtthread_join(th, NULL);
	}
	printf("worker%ld ends\n", id);
}

int main()
{
	gtthread_t th;

	gtthread_init(1000);
	gtthread_create(&th, worker, (void*) 1);
	gtthread_join(th, NULL);
	return 0;
}
