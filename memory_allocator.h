//
// Created by kanishka on 23/2/26.
//

#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include<iostream>
#include<sys/mman.h>

void* heap = NULL;

struct meta {
    size_t size;
    bool free;
    meta* next;
};

void alloc(size_t req_size);

#endif //MEMORY_ALLOCATOR_H
