//
// Created by kanishka on 23/2/26.
//

#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#define CAPACITY 64000

char heap[CAPACITY]{};

void heap_free(void *ptr);
void *heap_alloc(size_t size);


#endif //MEMORY_ALLOCATOR_H
