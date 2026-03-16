//
// Created by kanishka on 23/2/26.
//
#include "memory_allocator.h"

void alloc(size_t req_size) {
    if (heap == NULL) {
        heap = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); //we are passing address, size, permissions, private/shared flag, file descriptor and offset.
        meta *first_bloc = reinterpret_cast<meta*>(heap);
        first_bloc->free = 0;
        first_bloc->next = NULL;
        first_bloc->size = 4096;
    }

}