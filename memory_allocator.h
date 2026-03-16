//
// Created by kanishka on 23/2/26.
//

#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include<iostream>
#include<sys/mman.h>


struct meta {
    size_t size; //size of user memory
    bool free;
    meta *next; //ptr to next meta block
    meta *prev;
};


meta* find_free(size_t req_size, meta* start);
void createMeta(size_t req_size, meta* current);
void* alloc(size_t req_size);
void free_space(meta* garbage);


#endif //MEMORY_ALLOCATOR_H
