#ifndef _syscall_cpp
#define _syscall_cpp


#include "syscall_c.h"
#include "../lib/mem.h"
#include "../lib/console.h"

void* operator new(size_t);
void operator delete (void*);
class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    static void dispatch ();
    bool getReady(){
        return inScheduler;
    }
    using Body=void(*)(void*);
     Body getBody(){
        return myHandle->getBody();
    }
    void join();

    //static int sleep (time_t);
protected:
    Thread ();
     virtual void run (){};
private:

    bool inScheduler;
    thread_t myHandle;
    static void threadwrap(void* thr);
};

class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};
class Console {
public:
    static char getc ();
    static void putc (char cr);
};










#endif //_syscall_cpp