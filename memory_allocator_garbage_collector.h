//
// Created by kanishka on 23/2/26.
//
#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <iostream>
#include <sys/mman.h>
#include <cstdint>
#include <pthread.h>   // needed for get_stack()

struct meta {
    size_t size; //size of user memory
    bool free;
    meta *next; //ptr to next meta block
    meta *prev;
    bool reachable = false;
};

extern meta* heap;

// allocator functions
meta* find_free(size_t req_size, meta* start);
void createMeta(size_t req_size, meta* current);
void* alloc(size_t req_size);
void free_memory(meta* garbage);

char *stack_high;
void* current_top;

// GC functions
void get_stack();
void mark();
void sweep();
void scan_stack();
void scan_heap();
void mark_meta(uint64_t temp);

#endif //MEMORY_ALLOCATOR_H