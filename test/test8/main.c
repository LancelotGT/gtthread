// Test8
// Mutex. Protect a global variable.

#include <stdio.h>
#include <gtthread.h>

#define NUM_THREADS 100

gtthread_mutex_t g_mutex;
int g_num = 0;

void* worker(void* arg)
{
	gtthread_mutex_lock(&g_mutex);
	++g_num;
	gtthread_mutex_unlock(&g_mutex);
}

int main()
{
	int i;
	gtthread_t threads[NUM_THREADS];

	gtthread_init(1000);

	gtthread_mutex_init(&g_mutex);

	for (i = 0; i < NUM_THREADS; ++i) {
		gtthread_create(&threads[i], worker, (void*) i);
	}

	for (i = 0; i < NUM_THREADS; ++i) {
		gtthread_join(threads[i], NULL);
	}

	if (g_num != NUM_THREADS) {
		fprintf(stderr, 
				"!ERROR! Wrong result! %d != %d\n",
				g_num, NUM_THREADS);
	}
	return 0;
}
