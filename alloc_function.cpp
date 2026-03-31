//
// Created by kanishka on 27/3/26.
//

#include "alloc_function.h"

#include <cassert>

void* alloc(size_t req_size) {
    //this is the function the user will call
    void *temp = allocate(req_size);
    if (temp) {
        //if there is enough space, this function will return the free block
        print_heap();
        return temp;
    }
    //TODO: make a wrapper function for these
    get_stack();
    scan_stack();
    scan_heap(heap);
    sweep();
    reset_reachable();
    //these functions together, find reachable block, mark them true, then sweep all blocks with reachable = false and then reset all reachable back to false
    void *new_free = allocate(req_size);
    if (new_free) {
        print_heap();
        return new_free;
    }
    //in case freeing memory did not create enough space for user's requested size, we request some from the operating system using sbrk system calll
    meta* new_memory = reinterpret_cast<meta *>(sbrk(4096));
    heap_break += 4096;
    meta* list = heap;
    //TODO: make function to merge heap
    while (list->next !=NULL) {
        list = list->next;
    }
    if (list->free) {
        list->size += 4096;
    }
    else {
        list->next = new_memory;
        new_memory->prev = list;
        new_memory->next = NULL;
        new_memory->size = 4096 - sizeof(meta);
        new_memory->free = true;
    }
   return alloc(req_size);
}

