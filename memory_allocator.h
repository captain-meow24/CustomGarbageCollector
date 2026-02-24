//
// Created by kanishka on 23/2/26.
//

#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include<iostream>
#include<cassert>

#define CAPACITY 64000
#define CHUNK_CAP 1024

char heap[CAPACITY]{};
size_t heap_size = 0;

class chunk {
public:
    void *start;
    size_t size;
};
chunk* heap_chunk[CHUNK_CAP]{};
size_t alloc_chunk_size = 0;

void *heap_alloc(size_t size);
void heap_free(void *ptr);


#endif //MEMORY_ALLOCATOR_H
