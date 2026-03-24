//
// Created by kanishka on 23/3/26.
//
#include<stdio.h>
#include <pthread.h>

#ifndef GARBAGE_COLLECTOR_H
#define GARBAGE_COLLECTOR_H

void get_stack(void* stack_base_addr, size_t &size);
void mark();
void sweep();

#endif //GARBAGE_COLLECTOR_H
