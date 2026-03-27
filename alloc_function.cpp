//
// Created by kanishka on 27/3/26.
//

#include "alloc_function.h"

void* alloc(size_t req_size) {
    void *temp = allocate(req_size);
    if (temp) {
        return temp;
    }
    else {

        get_stack();
        scan_stack();
        scan_heap();
        sweep();
        reset_reachable();
        meta *new_free = find_free(req_size,heap);
        if (new_free) {
            createMeta(req_size, new_free);
            return new_free + 1;
        }
    }
    return NULL;
}

