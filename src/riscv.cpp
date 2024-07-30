//
// Created by os on 6/28/23.
//

#include "../h/riscv.hpp"
#include "../test/printing.hpp"



enum CODES{

    MALLOC = 0x01,
    FREE  = 0x02,
    USER=0x03,
    THREAD_CREATE = 0x11,
    THREAD_EXIT = 0x12,
    DISPATCH = 0x13,
    JOIN = 0x14,
    THREAD_CPP=0x19,
    SEM_OPEN = 0x21,
    SEM_CLOSE = 0x22,
    SEM_WAIT = 0x23,
    SEM_SIGNAL = 0x24,
    GETC=0x41,
    PUTC=0x42,

};


void Riscv::handleSupervisorTrap() {

    /*
   Eksplicitno naglasavam da sam iskoristio deo koda sa snimaka vezbi u
   kojima je radjena sinhrona i asinhrona promena konteksta
    */

    uint64 scauseVar; //, sepcV; //, sstatus;
    uint64 volatile sstatus;
    uint64 volatile sepcV;
    uint64 a0, a1;
    uint64 a3, a2, a4, a5, a6, a7;
    __asm__ volatile ("mv %[r], a1" : [r]"=r"(a1));
    __asm__ volatile ("mv %[r], a2" : [r]"=r"(a2));
    __asm__ volatile ("mv %[r], a3" : [r]"=r"(a3));
    __asm__ volatile ("mv %[r], a4" : [r]"=r"(a4));
    __asm__ volatile ("mv %[r], a5" : [r]"=r"(a5));
    __asm__ volatile ("mv %[r], a6" : [r]"=r"(a6));
    __asm__ volatile ("mv %[r], a7" : [r]"=r"(a7));


    scauseVar=r_scause();


    if(scauseVar == 0x0000000000000008UL || scauseVar==0x0000000000000009UL){


        __asm__ volatile ("csrr %0, sepc" : "=r" (sepcV));
        sstatus = r_sstatus();


        a0=read_a0();

        // obradi sistemske pozive


        if(a0==FREE){
            a1=read_a1();
            void* loc=(void*)a1;
            int f= MemoryAllocator::grab_singleton().free(loc);
            //int f=__mem_free(loc);
            write_a0(f);
        }
        if (a0==USER){
            mc_sstatus(1<<8);
            __asm__ volatile ("csrw sepc, %0" : : "r" (sepcV + 4));
            return;

        }
        if(a0==THREAD_CREATE){  //// POPRAVI
            TCB** nit;
            //printString("Ulazi\n");
            __asm__ volatile("mv %[a1], a1" : [a1] "=r"(nit));
            using Body = void (*)(void*);
            Body a2; //// moze jer je lokalno
            __asm__ volatile ("mv %[a2], a2" : [a2] "=r"(a2));
            void* arg=(void*)a3;

            *nit= TCB::create_Thread(a2, arg);
            uint64 x;
            if(nit!= nullptr){
                x=0;
            }
            else{
                x=-1;
            }

            write_a0(x);
        }
        if(a0==THREAD_EXIT){
            uint64 x=TCB::exitTCB();
            write_a0(x);
        }
        if(a0==DISPATCH){

            TCB::dispatch();
        }
        if(a0==JOIN){
            TCB** nit;
            __asm__ volatile("mv %[a1], a1" : [a1] "=r"(nit));

            TCB::join(nit);
        }
        if(a0==THREAD_CPP){  //// POPRAVI
            //printString("ULAZI U 19 PREKID\n");
            //printString("Ulazi\n");
            TCB** nit;
            __asm__ volatile("mv %[a1], a1" : [a1] "=r"(nit));
            using Body = void (*)(void*);
            Body a2;
            __asm__ volatile ("mv %[a2], a2" : [a2] "=r"(a2));
            void* arg=(void*)a3;
            *nit=TCB::createTCB_noScheduler(a2, arg);
            uint64 x;
            if(nit!= nullptr){
                x=0;
            }
            else{
                x=-1;
            }

            write_a0(x);
            //__putc('\n');

        }
        //sem open
        if(a0==SEM_OPEN){
            uint64 init=Riscv::read_a2();
            uint64 s=Riscv::read_a1();
            Sem** handle=(Sem**)s;
            Sem* ptr= Sem::open_semaphore(init);
            *handle=ptr;

            /////
            uint64 x=0;
            if(!handle )x=-2;
            __asm__ volatile ("mv a0, %[x]" : : [x]"r"(x));


        }
            //sem close
         if(a0==SEM_CLOSE){
            uint64 s=Riscv::read_a1();
            Sem* handle=(Sem*)s;
            int x= Sem::close_semaphore(handle);
            Riscv::write_a0(x);
        }
            //sem wait
         if(a0==SEM_WAIT){
            uint64 h=Riscv::read_a1();
            Sem* handle=(Sem*)h;
            int x= Sem::wait_semaphore(handle);
            Riscv::write_a0(x);

        }
            //sem signal
         if(a0==SEM_SIGNAL){
            uint64 s=Riscv::read_a1();
            Sem* handle=(Sem*)s;
            int x= Sem::signal_semaphore(handle);
            Riscv::write_a0(x);
        }
        if(a0==MALLOC){
            uint64 ar=Riscv::read_a1();
            //zovi funciju
            //void *ret = MemoryAllocator::grab_singleton().malloc(ar);
            void* x;
            x= MemoryAllocator::grab_singleton().malloc(ar);
            __asm__ volatile ("mv a0, %[x]" : : [x]"r"(x)); /////x

        }
        if (a0==GETC){
            char a=__getc();
            Riscv::write_a0(a);

        }
        if (a0==PUTC){
            uint64 c=Riscv::read_a1();
            __putc(c);

        }

        //// sepc i sstat
        w_sstatus(sstatus);
        sepcV+=4;
        w_sepc(sepcV);
        mc_sip(SIP_SSIP);



    }
    else if(scauseVar==0x8000000000000001UL){
        ////  prekid od TIMERA
        /*
        TCB::timeSliceCounter++;
        if (TCB::timeSliceCounter >= DEFAULT_TIME_SLICE)
       {
           uint64 sepc = r_sepc();
           uint64 sstatus = r_sstatus();
           TCB::timeSliceCounter = 0;
            TCB::dispatch();
           w_sstatus(sstatus);
            w_sepc(sepc);
        }
         */
        mc_sip(SIP_SSIP);

    }
    else if(scauseVar==0x8000000000000009UL){
        console_handler();

    } else{
        //uint64 r=read_a0();
        printString("scause : ");
        printInt(scauseVar);
        __putc('\n');

        /*
        __putc('\n');
        __putc('u');
        __putc('n');
        __putc('k');
        __putc('n');
        __putc('o');
        __putc('w');
        __putc('n');
         */
    }


}

// mozda posebna slicna fja kao ova samo za korisnicki rezim koja nema ovo ms_sstatus
void Riscv::popSppSpie() {

    __asm__ volatile("csrw sepc, ra");
    //ms_sstatus(SSTATUS_SPP); //////
    __asm__ volatile("sret");


}