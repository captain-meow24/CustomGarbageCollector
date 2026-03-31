//
// Created by kanishka on 27/3/26.
//
#include "garbage_collector.h"


void get_stack() {
    //finds the current top and start(highest address) of the stack so we can iterate through it to find pointer to heap
    size_t size;
    char *stack_low;    //lowest address
    pthread_attr_t attr;
    pthread_getattr_np(pthread_self(), &attr);  //saving current running thread, that is main's resources into attr
    pthread_attr_getstack(&attr, (void **) &stack_low, &size);   //finding lowest address and stack size
    stack_high = stack_low + size;   //since most memory in the stack is empty (reserved for recursive calls), we check for pointers between current top and highest address, (stack low + size)
    pthread_attr_destroy(&attr);    //destroying attr to prevent resource leak
    asm volatile("mov %%rsp, %0" : "=r"(current_top));   //inline assembly to find the value of rsp register, that is current top
}

void scan_stack() {
    //this function scans every 8 bytes in the active stack
    char *temp = (char *) current_top;
    while (temp < stack_high) {
        uint64_t value = *(uint64_t *) temp;   //casts temp into an 8-byte integer pointer and then reads the memory at that address
        mark_meta(value);
        temp += 8;
    }
}

void scan_heap(meta* root) {
    //this function scans the heap for pointers to heap
    meta* list = root;
    while (list != NULL) {
        if (list->reachable) {
            char *start = (char *) list + sizeof(meta);
            char *end = start + list->size;

            for (char *curr = start; curr < end; curr += 8) {
                uint64_t val = *(uint64_t *) curr;
                mark_meta(val);
            }
        }
        list = list->next;
    }
}

void mark_meta(uint64_t temp) {
    //this function checks if the 8-byte value is a heap pointer
    meta* list = heap;
    if ((temp >= ((uintptr_t)heap + sizeof(meta) )) && temp < ((uintptr_t)heap_break)) {
        while (list != NULL ) {
            uintptr_t start = (uintptr_t)list + sizeof(meta);
            uintptr_t end = start + list->size;
            if ((temp >= start) && (temp < end) && list->reachable == false) {
                //we set reachable as true if we find a pointer to that block
                list->reachable = true;
                scan_block(list);
                break;
            }
            list = list->next;
        }
    }
}

void sweep() {
    //we free all blocks with reachable = false
    meta* current = heap;
    while (current != NULL) {
        meta* next = current->next;
        if (!current->reachable && !current->free) {
            free_memory(current);
        }
        current = next;
    }
}

void reset_reachable() {
    //we set all reachable back to false after sweep
    meta* temp = heap;
    while (temp!=NULL) {
        temp->reachable = false;
        temp=temp->next;
    }
}

void scan_block(meta* block) {
    //this block scans every reachable bloc if it contains any more heap pointers
    char* start = (char*)block + sizeof(meta);
    char* end = start + block->size;

    for (char* curr = start; curr < end; curr += 8) {
        uint64_t val = *(uint64_t*)curr;
        mark_meta(val);
    }
}