//
// Created by kanishka on 27/3/26.
//

#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H


#include <iostream>
#include <cstdint>
#include <pthread.h>


extern char *stack_high;
extern void* current_top;

void get_stack();
void mark();
void sweep();
void scan_stack();
void scan_heap();
void mark_meta(uint64_t temp);
void sweep();
void reset_reachable();


#endif //GARBAGE_COLLECTOR_H
