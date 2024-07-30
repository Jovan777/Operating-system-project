//
// Created by os on 6/28/23.
//

#include "../h/syscall_cpp.hpp"
#include "../test/printing.hpp"
#include "../lib/mem.h"

void* operator new(size_t size){
    //return __mem_alloc(size);
    return mem_alloc(size);

}

void operator delete(void * loc) {
    //__mem_free(loc);
    mem_free(loc);
}


Thread::Thread(void (*body)(void *), void *arg){
    thread_create_noScheduler(&myHandle,body,arg);
}

Thread::~Thread() {
    myHandle->setFinished(true);
    //__mem_free(this);
    mem_free(myHandle);

}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::start() {
    //printString("Usao ovde\n"); //ulazi
    Scheduler::put(myHandle);
    //printString("Izasao ovde\n"); //ulazi
    return 0;
}

void Thread::threadwrap(void *thr) {
    ((Thread*)thr)->run();

}

void Thread::join()
{
    if (myHandle)
        thread_join(&myHandle);
}

Thread::Thread() {

    thread_create_noScheduler(&myHandle, &threadwrap, this);
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

void Console::putc(char c) {
    __putc(c);
}

char Console::getc() {
    return __getc();
}