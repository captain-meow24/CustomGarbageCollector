//
// Created by kanishka on 23/2/26.
//

#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include<iostream>
#include<cassert>

#define CAPACITY 64000

char heap[CAPACITY]{};
size_t heap_size = 0;


void *heap_alloc(size_t size);
void heap_free(void *ptr);


#endif //MEMORY_ALLOCATOR_H
