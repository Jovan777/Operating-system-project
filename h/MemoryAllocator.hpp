
#ifndef PROJECT_BASE_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_MEMORYALLOCATOR_HPP


#include "../lib/hw.h"


class MemoryAllocator {

    /*

     Eksplicitno navodim da sam u izradi Memorijskog alokatora iskoristio delove
     implementacija iz zadataka koji su dosli na :

     1. zadatku, kolokvijuma iz Avgusta 2021
     i
     2. zadatku, na drugom kolokvijumu iz Septembra 2015

     Kodovi nisu prosto prekopirani iz resenja nego sam iskoristio
     delove tih kodova u izradi sopstvenog resenja



    */

    struct MemoryBlock{
        MemoryBlock* next;
        MemoryBlock* previous;
        size_t size;
    };



public:
    MemoryBlock *header=nullptr;
    MemoryBlock *tail=nullptr;

    void inicijalizuj();
    int spoji(MemoryBlock* temp);
    static MemoryAllocator& grab_singleton();
    void *malloc(size_t sz);
    int free(void* adr);

};

#endif //PROJECT_BASE_MEMORYALLOCATOR_HPP
