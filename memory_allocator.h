//
// Created by kanishka on 27/3/26.
//

#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H


#include <iostream>
#include <sys/mman.h>
#include <cstdint>
#include <unistd.h>

struct meta {   //creating a meta data block for each allocated or free block of memory in the heap
    size_t size;
    bool free;
    meta *next;
    meta *prev;
    bool reachable = false;
};

extern meta* heap;   //root node of double linked list where heap address begins

extern char* heap_break;

// allocator functions
meta* find_free(size_t req_size, meta* start);
void createMeta(size_t req_size, meta* current);
void* allocate(size_t req_size);
void free_memory(meta* garbage);
void print_heap();


#endif //MEMORY_ALLOCATOR_H
