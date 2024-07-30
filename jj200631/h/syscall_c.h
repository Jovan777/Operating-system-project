
#ifndef SYSCALL_C_HPP
#define SYSCALL_C_HPP

#include "../lib/console.h"
#include "../lib/hw.h"
#include "../lib/mem.h"
#include "riscv.hpp"

//fje koje je potrebno implementirati u c-api


void* mem_alloc(size_t size);

int mem_free(void* loc);


class TCB;
typedef TCB* thread_t;

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_create_noScheduler(thread_t* handle, void(*start_routine)(void*), void* arg);


int thread_exit();

void thread_dispatch();

void thread_join(thread_t* handle);

class Sem;
typedef Sem* sem_t;


int sem_wait(sem_t id);

int sem_signal(sem_t id);
///
int sem_open(sem_t* handle, unsigned  init);
int sem_close(sem_t handle);


typedef unsigned long time_t;
int time_sleep(time_t);

void enterUser();

const int EOF=-1; /////
char getc();
void putc(char);


#endif //SYSCALL_C_HPP