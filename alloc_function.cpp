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
        void *new_free = allocate(req_size);
    return new_free;
}

