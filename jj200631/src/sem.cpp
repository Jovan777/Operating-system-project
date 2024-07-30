//
// Created by os on 8/9/23.
//

#include "../h/sem.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.h"

Sem* Sem::open_semaphore(unsigned int init) {
    Sem* s=(Sem*) MemoryAllocator::grab_singleton().malloc((sizeof(Sem) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
    if(s== nullptr){
        return nullptr; ///
    }
    s->tail=s->head= nullptr;
    s->init=init;
    return s;

}



int Sem::close_semaphore(Sem *handle) {

    if(handle== nullptr){
        return -2;
    }
    while (handle->head){ ///
        TCB* nit=handle->head->thread;
        nit->setBlocked(false);
        Scheduler::put(nit);
        Node* stari=handle->head;
        handle->head=handle->head->next;
        //__mem_free(stari);
        MemoryAllocator::grab_singleton().free(stari);
        if(handle->head== nullptr){
            handle->tail= nullptr;
        }

    }
    //__mem_free(handle);
    MemoryAllocator::grab_singleton().free(handle);
    handle= nullptr;
    //printString("Zavrsio");
    return 0;

}



int Sem::wait_semaphore(Sem *handle) {


    if(!handle){
        return -3;
    }
    --handle->init;
    if (handle->init<0){
        if (TCB::running->finished){
            return 0;
        }
        TCB* old=TCB::running;
        old->setBlocked(true);
        Node* temp=(Node*) MemoryAllocator::grab_singleton().malloc((sizeof(Node) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
        temp->thread=old;
        temp->next= nullptr;
        if (!handle->head){
            handle->head=temp;
        }
        else{
            handle->tail->next=temp;
        }
        handle->tail=temp;
        thread_dispatch();


    }

    return 0;

}

int Sem::signal_semaphore(Sem *handle) {

    if (handle== nullptr){
        return -4;
    }
    ++handle->init;
    if(handle->init<=0){
        TCB* nit=handle->head->thread;
        Scheduler::put(nit);
        nit->setBlocked(false);
        Node* stari=handle->head;
        handle->head=handle->head->next;
        if (handle->head== nullptr){
            handle->tail= nullptr;
        }
        //__mem_free(stari);
        MemoryAllocator::grab_singleton().free(stari);
        //printString("Dao signal\n");

    }

    return 0;
}
