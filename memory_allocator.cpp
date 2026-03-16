//
// Created by kanishka on 23/2/26.
//
#include "memory_allocator.h"

meta *heap = NULL;

void createMeta(size_t req_size, meta* current) {

    meta* newm = reinterpret_cast<meta*>((char*)current + req_size + sizeof(meta));
    newm->next = current->next;
    if (newm->next) {
        newm->next->prev = newm;
    }
    newm->free = true;
    newm->size = current->size - req_size - sizeof(meta);
    newm->prev = current;
    current->next = newm;
    current->size = req_size;
    current->free = false;
}

void* alloc(size_t req_size) {
    if (req_size%8) {
        size_t alignm = req_size%8;  //aligning to 8 bits
        alignm = 8 - alignm;
        req_size += alignm;
    }
    if (heap == NULL) {
        heap = reinterpret_cast<meta*>(mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
        //we are passing address, size, permissions, private/shared flag, file descriptor and offset.
        heap->prev = NULL;
        heap->size = 4096 - sizeof(meta);
        heap->next = NULL;
        createMeta(req_size, heap);
        return heap+1;
    }
    else {
        return (void*)((char*)heap + sizeof(meta));
    }

}