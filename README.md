## GTThread--A User Level Thread Library
GTthread is a user level thread library. It has similar semantics as pthread library. The library runs purely on user mode and it create multiple threads by user level context switching. The advantage of having a user level threads is that we don't need to go to kernel on every context switch. Hence user level threads are more efficient.

## Semantics
The API is very similar to pthread, except that gtthread_init(period) must be called before creating any thread, where period is the time interval in useconds between context swich. gtthread also does not have detach. All threads are joinable.
  
## What Linux platform do I use.           
I am using ubuntu/trusty64 (Official Ubuntu Server 14.04 LTS builds) created by vagrant.

## How to compile the library and run my program
To compile, run
```
mkdir include && mkdir lib
cd src && make
```
It will simply put the header files (gtthread.h steque.h) into include folder and library file (libgtthread.a) into lib folder. 
 
## How the preemptive scheduler is implemented.
* The context switch is implemented using two things. One is the SIGVTALRM alarm signal. Every thread has some time do its work. Once the time is used up, an alarm signal will be delivered and switch to another thread. The other thing is the user level thread switching is done by syscalls like setcontext, getcontext, swapcontext and makecontext.
 
* getcontext is used whenever a new thread is created. We use getcontext to save the stack frame, register values and program counter associated with the current thread. Then we use makecontext to associate the thread with their start_routine.

* swapcontext is used in context switching. It saves the context for current thread and swtich to and run the start_routine of the next thread. setcontext is only used when a thread has exited or is terminated. In this case, we do not need to save the current context.

## How I prevent deadlocks in my dining philosopher solution.
I use a simple strategy to prevent deadlocks. Every philosopher has a index and every chopstick also has an index. For instance, the index of left chopstick is (phil_id + 4) % 5, and the index of the right chopstick is phil_id. We can just let every philosopher pick up the chopstick with the smaller index. In this way, the deadlock situation where every philopher picks up the chopstick on the side will never happends, since a guy will not pick any chopstick in this case.
