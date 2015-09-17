// Test2
// Join multiple threads, and check values returned by gtthread_exit.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gtthread.h>

#define NUM_THREADS 100

long g_return_values[NUM_THREADS];

void* worker(void* arg)
{
	long i = (long) arg;
	gtthread_exit((void*) g_return_values[i]);
	return NULL;
}

int main()
{
	gtthread_t threads[NUM_THREADS];
	void* rets[NUM_THREADS];
	int i = 0;

	srand(time(NULL));

	// Randomly assign return values
	for (i = 0; i < NUM_THREADS; ++i) {
		g_return_values[i] = rand();
	}

	gtthread_init(1000);

	for (i = 0; i < NUM_THREADS; ++i) {
		gtthread_create(&threads[i], worker, (void*) i);
	}

	for (i = 0; i < NUM_THREADS; ++i) {
		gtthread_join(threads[i], &rets[i]);
	}

	// Check return values
	for (i = 0; i < NUM_THREADS; ++i) {
		if ((long) rets[i] != g_return_values[i]) {
			fprintf(stderr, 
					"!ERROR! %dth return value is wrong! %d, %d\n",
					i, rets[i], g_return_values[i]);
		}
	}

	return 0;
}
