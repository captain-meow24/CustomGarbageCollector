//
// Created by kanishka on 27/3/26.
//

#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H


#include <iostream>
#include <cstdint>
#include <pthread.h>
#include "memory_allocator.h"


extern char *stack_high;
extern void* current_top;

void get_stack();
void mark();
void sweep();
void scan_stack();
void scan_heap(meta* root);
void mark_meta(uint64_t temp);
void reset_reachable();
void scan_block(meta* block);


#endif //GARBAGE_COLLECTOR_H
