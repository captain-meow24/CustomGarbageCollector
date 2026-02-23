//
// Created by kanishka on 23/2/26.
//
#include "memory_allocator.h"

void *heap_alloc(size_t size) {
    assert(heap_size + size <= CAPACITY);
    void *result = heap_size + size;

}
