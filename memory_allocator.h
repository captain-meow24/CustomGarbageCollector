//
// Created by kanishka on 27/3/26.
//

#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H


#include <iostream>
#include <sys/mman.h>
#include <cstdint>

struct meta {
    size_t size;
    bool free;
    meta *next;
    meta *prev;
    bool reachable = false;
};
extern int syscall_flag;

extern meta* heap;

// allocator functions
meta* find_free(size_t req_size, meta* start);
void createMeta(size_t req_size, meta* current);
void* allocate(size_t req_size);
void free_memory(meta* garbage);
//void print_heap();


#endif //MEMORY_ALLOCATOR_H
