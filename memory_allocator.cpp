//
// Created by kanishka on 23/2/26.
//
#include "memory_allocator.h"

void *heap_alloc(size_t size) {
    assert(heap_size + size <= CAPACITY);
    void *result = heap + heap_size;
    heap_size += size;
    assert(alloc_chunk_size + 1 < CHUNK_CAP);
    chunk *chunk1 = new chunk();
    chunk1->start = result;
    chunk1->size = size;
    alloc_chunk_size ++;
    heap_chunk[alloc_chunk_size] = chunk1;

    return 0;

}
