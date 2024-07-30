#ifndef TCB_HPP
#define TCB_HPP


#include "riscv.hpp"
#include "../lib/hw.h"
#include "../lib/mem.h"
#include "Scheduler.hpp"
#include "MemoryAllocator.hpp"

typedef TCB* thread_t;


class TCB{

    /*
    Eksplicitno naglasavam da sam iskoristio deo koda sa snimaka vezbi u
    kojima je radjena sinhrona i asinhrona promena konteksta
     */




    using Body=void(*)(void*);
    struct Context
    {
        uint64 ra;
        uint64 sp;
    };
    static uint64 stack_size;
    static uint64 TIME;
    static uint64 timeSliceCounter;

    Context context;
    Body body;
    void* arg;
    bool finished;
    bool blocked;
    uint64* stek;
    uint64 timeSlice;


    TCB(Body body_f, void* argument, uint64 timeS){
        timeSlice=timeS;
        finished= false;
        blocked= false;
        body=body_f;
        arg=argument;
        stek=(body != nullptr ?
              (uint64*) MemoryAllocator::grab_singleton().malloc(DEFAULT_STACK_SIZE * sizeof(uint64)) :
              nullptr);

        context.ra = (uint64) &threadWrapper;
        context.sp =(stek != nullptr ? (uint64) &stek[DEFAULT_STACK_SIZE] : 0);
    };





    static void threadWrapper();

public:

    static TCB *running; // uvek samo jedna ce biti running
    static void join(thread_t* handle);

    friend class Riscv;
    friend class Sem;
    friend class Scheduler2;
    void* operator new(size_t);
    void operator delete(void*);
    Body getBody(){
        return body;
    }

    static TCB* create_Thread(Body body, void* arg);
    static TCB* createTCB_noScheduler(Body body, void* arg);

    static void dispatch();
    static void yield();
    static void contextSwitch(Context *oldContext, Context *runningContext);
    static int exitTCB();
    static void join(TCB* nit);

    bool isFinished();
    void setFinished(bool f);
    bool isBlocked();
    void setBlocked(bool  b);


    /*

     Kad proradi ovo sve onda dodaj semafore

    */
    // dodaj semafore

};
//////




#endif //TCB_HPP