// Test1
// Init, create, and join a thread.

#include <stdio.h>
#include <string.h>
#include <gtthread.h>

const char* g_str = "hello world\n";
const long g_ret = 0xF0F0F0F0;

void* worker(void* arg)
{
	if (strcmp((char*) arg, g_str) != 0) {
		fprintf(stderr, 
				"!ERROR! Wrong argument! %p, %p\n",
				arg, g_str);
	}
	return (void*) g_ret;
}

int main()
{
	gtthread_t th1;
	void* ret;

	gtthread_init(1000);

	gtthread_create(&th1, worker, (void*) g_str);

	gtthread_join(th1, &ret);

	if ((long) ret != g_ret) {
		fprintf(stderr, 
				"!ERROR! Wrong return! %x, %x\n",
				ret, g_ret);
	}

	return 0;
}
