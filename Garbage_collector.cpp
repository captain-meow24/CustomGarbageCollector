//
// Created by kanishka on 23/3/26.
//

#include "Garbage_collector.h"


void get_stack() {
    size_t size;
    char* stack_base_addr;  //this gives the top of the stack memory, which is mostly unused
    pthread_attr_t attr;  //attr will store all the meta-data about the running thread, i.e main thread
    pthread_getattr_np(pthread_self(), &attr);    //pthread_self(0 will call the current thread
    pthread_attr_getstack(&attr, (void**)&stack_base_addr,&size);   //void** means it points tos something unknown, it can hold any type
    stack_top = stack_base_addr + size;
    pthread_attr_destroy(&attr);   //destroying attr to avoid resources leak
    asm volatile("mov %%rsp, %0" : "=r"(current_base));
}

void scan_stack() {

    char* temp = stack_top;
    while (temp< current_base) {
        uint64_t value = *(uint64_t*)temp;

        temp += 8;
    }
}

void mark_meta(uint64_t temp) {
    size_t offset = (8 - (sizeof(meta) % 8)) % 8;
    meta* list = heap;
    if ((temp >= ((uintptr_t)heap + sizeof(meta) + offset )) && temp < ((uintptr_t)heap + 4096)) {
        while (list != NULL) {
            uintptr_t start = (uintptr_t)list + sizeof(meta) + offset;
           uintptr_t end   = start + list->size;
            if ((temp >= start) && (temp < end)) {
                list->reachable = true;
                break;
            }
            list = list->next;
        }
    }
    //This function goes to every block in the heap linked list and checks if the temp value lies in the the address range of any meta block
}