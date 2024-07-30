//
// Created by os on 6/26/23.
//

#include "../h/MemoryAllocator.hpp"

/*

     Eksplicitno navodim da sam u izradi Memorijskog alokatora iskoristio delove
     implementacija iz zadataka koji su dosli na :

     1. zadatku, kolokvijuma iz Avgusta 2021
     i
     2. zadatku, na drugom kolokvijumu iz Septembra 2015

     Kodovi nisu prosto prekopirani iz resenja nego sam iskoristio
     delove tih kodova u izradi sopstvenog resenja



    */


MemoryAllocator& MemoryAllocator::grab_singleton() {
    static MemoryAllocator singleton;
    return singleton;
}


void MemoryAllocator::inicijalizuj() {
    header=(MemoryBlock*)((uint64*)HEAP_START_ADDR);

    uint64* start=(uint64*)HEAP_START_ADDR;
    uint64* end=(uint64*)HEAP_END_ADDR;

    header->size= ((uint64)(end-start-sizeof (MemoryAllocator))) / MEM_BLOCK_SIZE; // radi
    header->previous= header->next= nullptr;


}

void *MemoryAllocator::malloc(size_t sz) {
    if(sz<0){ ////
        return nullptr;
    }

    MemoryBlock* curr=header, *pret= nullptr;
    for(; curr ; pret=curr, curr=curr->next){
        if(curr->size >= sz){
            if(curr->size == sz ){
                if(!pret){
                    header=curr->next;
                }
                else {
                    pret->next = curr->next;
                }
                if(curr->next){
                    curr->next->previous =pret;
                }
                curr->size = sz;
                return (uint64 *)curr + sizeof(MemoryBlock);
            }
            size_t offset= MEM_BLOCK_SIZE * sz;
            MemoryBlock* novi=(MemoryBlock*)((uint64 *)curr + offset);
            if(pret== nullptr){
                header=novi;
            }
            else {
                pret->next=novi;
            }
            if(curr->next!= nullptr){
                curr->next->previous=novi;
            }
            novi->size= curr->size - sz;
            novi->next=curr->next;
            novi->previous=pret;
            curr->size = sz;
            return sizeof(MemoryBlock) + (uint64 *)curr;
        }
    }
    return nullptr; // ok


}

int MemoryAllocator::spoji(MemoryBlock *temp) {

    if (!temp){
        return 0;
    }

    if (temp->next!= nullptr && (char*)(temp->next)==((char*)temp+temp->size*MEM_BLOCK_SIZE)){
        temp->size=temp->size+temp->next->size;
        temp->next=temp->next->next;
        if (temp->next!= nullptr) {
            temp->next->previous = temp;
        }
        return 1;
    }
    return 0;

}

int MemoryAllocator::free(void *location) {

    if(location== nullptr){ /////
        return 0;
    }

    if ((uint64 *)location<(uint64 *)HEAP_START_ADDR){
        return -1;
    }
    if((uint64 *)location>(uint64*)HEAP_END_ADDR){
        return -2;
    }


    MemoryBlock* temp;
    if( (char*)header>(char*)location || !header ){
        temp= nullptr;
    }
    else{
        for(temp=header; (char*)temp->next < (char*)location && temp!= nullptr ;){
            temp=temp->next;
        }

    }

    MemoryBlock* novi=(MemoryBlock*)((uint64*)location - sizeof(MemoryBlock));
    novi->previous=temp; // temp je mozda nullptr
    if (!temp){
        novi->next=header;

    }
    else{
        novi->next=temp->next;

    }
    if (novi->next!= nullptr){
        novi->next->previous=novi;
    }
    if (!temp){

        header=novi;

    }
    else{

        temp->next=novi;

    }

    spoji(novi);
    spoji(temp);
    return 0;


}




