//
// Created by os on 6/28/23.
//


#include "../h/Scheduler.hpp"


Scheduler::Node* Scheduler::head= nullptr;
Scheduler::Node* Scheduler::tail= nullptr;

TCB* Scheduler::get() {


    if (head== nullptr) {
        return nullptr;
    }

    Node *elem = head;
    head = head->next;
    if (head== nullptr) { tail = 0; }

    TCB *thread = elem->nit;
    MemoryAllocator::grab_singleton().free(elem);
    return thread;
}

void Scheduler::put(TCB* b) {  //// POPRAVI
    Node* novi= (Node*) MemoryAllocator::grab_singleton().malloc(sizeof(Node));
    novi->next=0;
    novi->nit=b;


    if(head){
        tail->next=novi;
    }
    else{
        head=tail=novi;
    }
    tail=novi;


}