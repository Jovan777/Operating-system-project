//
// Created by os on 6/28/23.
//
#include "../h/syscall_c.h"
#include "../lib/console.h"
#include "../test/printing.hpp"

void* mem_alloc(size_t size){

    size_t s=(size+ MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;
    Riscv::write_a1(s);
    __asm__ volatile("li a0,0x01");
    __asm__ volatile ("ecall");
    uint64 x=Riscv::read_a0();
    return (void*) x;

    //return __mem_alloc((size+ MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE); //radi za proveru


};

int mem_free(void* loc){
    void* x=loc;  //// probaj sa uint64 kasnije
    __asm__ volatile ("mv a1, %[x]" : : [x] "r" (x));
    Riscv::write_a0(0x02);

    __asm__ volatile ("ecall");

    return Riscv::read_a0(); ////
};



class TCB;  ///
typedef TCB* thread_t;

//// argumenti moraju od kraja

//// popravi
int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg){
    __asm__ volatile("mv a3,%[arg]": :[arg]"r" (arg));
    __asm__ volatile("mv a2,%[start_routine]": :[start_routine]"r" (start_routine));
    __asm__ volatile("mv a1,%[handle]": :[handle]"r" (handle));


    Riscv::write_a0(0x11);
    __asm__ volatile("ecall");

    return Riscv::read_a0();


};

//// sve ok

int thread_create_noScheduler(thread_t* handle, void(*start_routine)(void*), void* arg){
    __asm__ volatile("mv a3,%[arg]": :[arg]"r" (arg));
    __asm__ volatile("mv a2,%[start_routine]": :[start_routine]"r" (start_routine));
    __asm__ volatile("mv a1,%[handle]": :[handle]"r" (handle));


    Riscv::write_a0(0x19);
    __asm__ volatile("ecall");

    return Riscv::read_a0();


};

int thread_exit(){
    Riscv::write_a0(0x12);
    __asm__ volatile("ecall");

    uint64 x=Riscv::read_a0();
    return x; ////
};


void thread_dispatch(){

    Riscv::write_a0(0x13);
    __asm__ volatile("ecall");
    ////
};


void thread_join(thread_t* handle){
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    Riscv::write_a0(0x14);
    __asm__ volatile("ecall");
};


///// DEO ZA SEMAFORE


int sem_open(sem_t *handle, unsigned int init) {


    __asm__ volatile("mv a2,%[init]"::[init]"r"(init));
    __asm__ volatile("mv a1,%[handle]"::[handle]"r"(handle));
    Riscv::write_a0(0x21);
    __asm__ volatile("ecall");
    return Riscv::read_a0();


}

int sem_close(sem_t handle) {
    __asm__ volatile("mv a1,%[handle]"::[handle]"r"(handle));
    Riscv::write_a0(0x22);
    __asm__ volatile("ecall");
    return Riscv::read_a0();

}

int sem_wait(sem_t id) {
    __asm__ volatile("mv a1,%[handle]"::[handle]"r"(id));
    Riscv::write_a0(0x23);
    __asm__ volatile("ecall");
    return Riscv::read_a0();

}

int sem_signal(sem_t id) {
    __asm__ volatile("mv a1,%[handle]"::[handle]"r"(id));
    Riscv::write_a0(0x24);
    __asm__ volatile("ecall");
    return Riscv::read_a0();

}


char getc(){

    /*
    char c=__getc();
    return c;
    //radi za proveru
    */
    Riscv::write_a0(0x41);
    __asm__ volatile("ecall");
    char c=Riscv::read_a0();
    return c;
};

void enterUser(){
    __asm__ volatile("li a0,0x03");
    __asm__ volatile ("ecall");
}


void putc(char c){

    //__putc(c); //radi za proveru
    char a=c;
    Riscv::write_a1(a);
    Riscv::write_a0(0x42);
    __asm__ volatile("ecall");

};


