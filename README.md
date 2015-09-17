## GTThread--A User Level Thread Library
GTthread is a user level thread library. It has similar semantics as pthread library. The library runs purely on user mode and it create multiple threads by user level context switching. The advantage of having a user level threads is that we don't need to go to kernel on every context switch. Hence user level threads are more efficient.

## Semantics
The API is very similar to pthread, except that gtthread_init(period) must be called before creating any thread, where period is the time interval in useconds between context swich. gtthread also does not have detach. All threads are joinable.

## Implementation
The context switch is implemented using two things. One is the SIGVTALRM alarm signal. Every thread has some time do its work. Once the time is used up, an alarm signal will be delivered and switch to another thread. The other thing is the user level thread switching is done by syscalls like setcontext, getcontext, swapcontext and makecontext.

## Limitation
gtthread is a user level thread library, meaning that it has no way to access many resources that are only available to kernel. For example, gtthread can only be scheduled on a processor. In a multi-core architecture, if a thread is waiting to be scheduled and there is a free processor, pthread scheduler is able to schedule this thread to another processor while gtthread cannot.
