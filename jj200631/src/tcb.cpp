//
// Created by os on 6/28/23.
//

#include "../h/tcb.hpp"
#include "../test/printing.hpp"



uint64 TCB::stack_size=DEFAULT_STACK_SIZE;
uint64 TCB::TIME=DEFAULT_TIME_SLICE;
TCB* TCB::running= nullptr;
uint64 TCB::timeSliceCounter=0;


/*
 *
  Eksplicitno naglasavam da sam iskoristio deo koda sa snimaka vezbi u
  kojima je radjena sinhrona i asinhrona promena konteksta

 *
 */

bool TCB::isBlocked() {
    return blocked;
}
bool TCB::isFinished() {
    return finished;
}
void TCB::setFinished(bool f) {
    finished=f;
}
void TCB::setBlocked(bool b) {
    blocked=b;
}

void TCB::dispatch() {

    TCB *old = running;
    if (!old->isFinished() && !old->blocked) { Scheduler::put(old); }
    running = Scheduler::get();


    TCB::contextSwitch(&old->context, &running->context);
}

TCB* TCB::create_Thread(TCB::Body body, void *arg) {
    TCB* nit=(TCB*) MemoryAllocator::grab_singleton().malloc
            ((sizeof(TCB) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    nit->body=body;
    nit->arg=arg;
    nit->timeSlice=DEFAULT_TIME_SLICE;
    nit->blocked=nit->finished= false;
    nit->stek=(uint64*)(body!= nullptr ? MemoryAllocator::grab_singleton().malloc(
            ((sizeof(uint64) * DEFAULT_STACK_SIZE) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE)
                                       : nullptr);
    nit->context= {
            (uint64)&threadWrapper, nit->stek != nullptr ?
                                    (uint64) &(nit->stek[stack_size]) : 0
    };

    //// provera za main
    if(body!= nullptr){
        Scheduler::put(nit);
    }

    return nit;
};

TCB* TCB::createTCB_noScheduler(TCB::Body body, void *arg) {
    //printString("Ulazi no sch\n");
    TCB* nit=(TCB*) MemoryAllocator::grab_singleton().malloc((sizeof(TCB) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    //TCB* nit=(TCB*)__mem_alloc((sizeof(TCB)+MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE);


    nit->body=body;
    nit->arg=arg;
    nit->timeSlice=DEFAULT_TIME_SLICE;
    nit->blocked=nit->finished= false;
    nit->stek=(uint64*)(body!= nullptr ? MemoryAllocator::grab_singleton().malloc(
            ((sizeof(uint64) * DEFAULT_STACK_SIZE) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE)
                                       : nullptr);
    nit->context= {
            (uint64)&threadWrapper, nit->stek != nullptr ?
                                    (uint64) &(nit->stek[stack_size]) : 0
    };


    //// isto kao obicna samo mi treba za CPP API

    return nit;

}

void TCB::join(thread_t* nit)
{
    while(!(*nit)->isFinished())
        TCB::dispatch();
}


void TCB::threadWrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    running->setFinished(true);
    dispatch();
}

void TCB::yield() {

    Riscv::write_a0(0x13);
    __asm__ volatile ("ecall");
    //ret=Riscv::read_a0();
    //printInt(ret);
    return;
}

int TCB::exitTCB() {
    if (running != nullptr) {
        running->setFinished(true); // postavi na finish i nece je ubaciti u sch
        dispatch();
        return 0;
    }
    return 1;
}

