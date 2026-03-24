//
// Created by kanishka on 23/3/26.
//

#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

#include <pthread.h>
#include "memory_allocator.h"

char *stack_top;   //this is where the stack memory starts storing, this is the highest address as stack grows towards lower memory address
void* current_top;

void get_stack();  //gets size and bottom address of the stack
void mark();
void sweep();
void scan_stack();
void scan_heap();

#endif //GARBAGE_COLLECTOR_H
