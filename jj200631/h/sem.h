//
// Created by os on 8/9/23.
//

#ifndef PROJECT_BASE_2023_SEM_H
#define PROJECT_BASE_2023_SEM_H

#include "tcb.hpp"

class Sem {

public:
    struct Node{
        TCB* thread;
        Node* next=nullptr;
    };

    static int wait_semaphore(Sem* handle);

    static int signal_semaphore(Sem* handle);
    static Sem* open_semaphore(unsigned init);

    static int close_semaphore(Sem* handle);

    bool empty(){
        return head== nullptr;
    }



private:

    int init;

    friend class Riscv;
    friend class TCB; ////

    Node* head;
    Node* tail;

};


#endif //PROJECT_BASE_2023_SEM_H
