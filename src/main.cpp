//
// Created by os on 6/24/23.
#include "../lib/hw.h"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/riscv.hpp"
//#include "../test_moj/Threads_C_API_test.hpp"
//#include "../test_moj/System_Mode_test.hpp"
//#include "../test_moj/Threads_CPP_API_test.hpp"
#include "../h/syscall_c.h"
#include "../h/syscall_cpp.hpp"
//#include "../test_moj/ConsumerProducer_C_API_test.hpp"
//#include "../test_moj/ConsumerProducer_CPP_Sync_API_test.hpp"
//#include "../test/userMain.cpp"

void userMain();

void wrapperTest(void*){
    //Threads_C_API_test();
    //System_Mode_test();
    //Threads_CPP_API_test();
    //producerConsumer_C_API();
    //producerConsumer_CPP_Sync_API();
    userMain();

}



int main(){



    // postavljanje default prekidne runtine
    Riscv::w_stvec((uint64)&Riscv::supervisorTrap);
    Riscv::mc_sstatus(Riscv::SSTATUS_SIE); ///////////
    MemoryAllocator::grab_singleton().inicijalizuj();

    //__putc('a');
    //enterUser();
    //__putc('\n');

    TCB* niti[2];
    niti[0]= TCB::create_Thread(nullptr, nullptr);
    niti[1]= TCB::create_Thread(&wrapperTest, nullptr);
    //thread_create(&niti[0], nullptr, nullptr);
    //thread_create(&niti[1], &wrapperTest, nullptr);


    TCB::running=niti[0];


    while (!(niti[1]->isFinished())){
        TCB::yield();
    }


    __putc('\n');
    __putc('o');
    __putc('k');
    __putc('\n');
    return 0;
}




//



