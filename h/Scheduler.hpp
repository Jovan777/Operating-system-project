#ifndef SCHEDULER_NOVI_HPP
#define SCHEDULER_NOVI_HPP

#include "../h/MemoryAllocator.hpp"
#include "tcb.hpp"
#include "../lib/mem.h"

class TCB;
//FIFO SCHEDULER
class Scheduler{

public:
    struct Node{
        TCB* nit;
        Node* next;
    };

private:
    static Node* head;
    static Node* tail;


public:
    static void put(TCB* nit);
    static TCB* get();


};


#endif //SCHEDULER_NOVI_HPP