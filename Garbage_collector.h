//
// Created by kanishka on 23/3/26.
//

#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

#include <pthread.h>
#include "memory_allocator.h"
#include <cstdint>

char *stack_top;   //this is where the stack memory starts storing, this is the highest address as stack grows towards lower memory address
void* current_base;   //this will store the starting address of stack/highest address

void get_stack();  //gets size and bottom address of the stack
void mark();
void sweep();
void scan_stack();
void scan_heap();
void compare_heap(uint64_t temp);

#endif //GARBAGE_COLLECTOR_H
