//
// Created by kanishka on 27/3/26.
//

#include "alloc_function.h"

void* alloc(size_t req_size) {
    void *temp = allocate(req_size);
    if (temp) {
        return temp;
    }
        get_stack();
        scan_stack();
        scan_heap(heap);
        sweep();
        reset_reachable();
        void *new_free = allocate(req_size);
    if (new_free) {
        return new_free;
    }
    meta* new_memory = reinterpret_cast<meta *>(mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    syscall_flag++;
    meta* list = heap;
    while (list->next!=NULL) {
        list = list->next;
    }
    new_memory->prev = list;
    new_memory->free = true;
    new_memory->size = 4096 - sizeof(meta);
    new_memory->next = NULL;
    list->next = new_memory;
    createMeta(req_size, new_memory);
    return reinterpret_cast<char*>(new_memory + 1) ;
}

