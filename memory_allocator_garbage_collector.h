//
// Created by kanishka on 23/2/26.
//
#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <iostream>
#include <sys/mman.h>
#include <cstdint>
#include <pthread.h>

struct meta {
    size_t size;
    bool free;
    meta *next;
    meta *prev;
    bool reachable = false;
};

extern meta* heap;

// allocator functions
meta* find_free(size_t req_size, meta* start);
void createMeta(size_t req_size, meta* current);
void* alloc(size_t req_size);
void free_memory(meta* garbage);

extern char *stack_high;
extern void* current_top;

// GC functions
void get_stack();
void mark();
void sweep();
void scan_stack();
void scan_heap();
void mark_meta(uint64_t temp);
void sweep();
void reset_reachable();

#endif //MEMORY_ALLOCATOR_H