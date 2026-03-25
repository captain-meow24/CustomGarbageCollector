//
// Created by kanishka on 23/2/26.
//
#include "memory_allocator_garbage_collector.h"


meta *heap = NULL;

meta *find_free(size_t req_size, meta *start) {
    meta *temp = start;
    while (temp != NULL) {
        if (temp->free && temp->size >= (req_size + sizeof(meta))) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}


void createMeta(size_t req_size, meta *current) {
    meta *newm = reinterpret_cast<meta *>((char *) current + req_size + sizeof(meta));
    //making a new meta block where the requested user memory ends
    newm->next = current->next; //connecting the double linked list
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

void *alloc(size_t req_size) {
    size_t offset = 8 - (sizeof(meta) % 8);
    //size of the padding between meta chunk and user memory, to align start and end of the user memory by 8 bytes
    //this is done so that we can each word can be scanned and checked for pointer to heap, hence implementing mark and sweep for garbage collection
    if (offset == 8) {
        offset = 0;
    }

    if (req_size % 8) {
        size_t alignm = req_size % 8; //aligning user memory to 8 bytes
        alignm = 8 - alignm;
        req_size += alignm;
    }
    if (heap == NULL) {
        heap = reinterpret_cast<meta *>(mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
        //we are passing address, size, permissions, private/shared flag, file descriptor and offset.
        heap->prev = NULL;
        heap->free = true;
        heap->size = 4096 - sizeof(meta);
        heap->next = NULL;
        createMeta(req_size, heap);
        return (char *) (heap + 1) + offset;
    }
    meta *free_space = find_free(req_size, heap);
    if (free_space) {
        createMeta(req_size, free_space);
        return free_space + 1 + offset;
    }
    return NULL;
}

void free_memory(meta *garbage) {
    if (garbage->prev && garbage->next && garbage->prev->free && garbage->next->free) {
        garbage->prev->size += sizeof(meta) + sizeof(meta) + garbage->size + garbage->next->size;
        garbage->prev->next = garbage->next->next;
        if (garbage->next->next) {
            garbage->next->next->prev = garbage->prev;
        }
    } else if (garbage->prev && garbage->prev->free) {
        garbage->prev->size += garbage->size + sizeof(meta);
        garbage->prev->next = garbage->next;
        if (garbage->next) {
            garbage->next->prev = garbage->prev;
        }
    } else if (garbage->next && garbage->next->free) {
        garbage->next->prev = garbage->prev;
        garbage->next->size += sizeof(meta) + garbage->size;
        if (garbage->prev) {
            garbage->prev->next = garbage->next;
        }
    } else {
        garbage->free = true;
    }
}


void get_stack() {
    size_t size;
    char *stack_low; //this gives the top of the stack memory, which is mostly unused
    pthread_attr_t attr; //attr will store all the meta-data about the running thread, i.e main thread
    pthread_getattr_np(pthread_self(), &attr); //pthread_self(0 will call the current thread
    pthread_attr_getstack(&attr, (void **) &stack_low, &size);
    //void** means it points tos something unknown, it can hold any type
    stack_high = stack_low + size;
    pthread_attr_destroy(&attr); //destroying attr to avoid resources leak
    asm volatile("mov %%rsp, %0" : "=r"(current_top));
}

void scan_stack() {
    //this function scans stack for any pointers, 8 bytes at a time
    char *temp = (char *) current_top;
    while (temp < stack_high) {
        uint64_t value = *(uint64_t *) temp;
        mark_meta(value);
        temp += 8; //pointers are 8 byte aligned in x86 64
    }
}

void scan_heap() {
    //this function scans user accessable heap for any pointers
    size_t offset = (8 - (sizeof(meta) % 8)) % 8;
    meta *list = heap;
    while (list != NULL) {
        char *start = (char *) list + offset + sizeof(meta);
        char *end = start + list->size;
        for (char *curr = start; curr < end; curr += 8) {
            uint64_t val = *(uint64_t *) curr;
            mark_meta(val);
        }
        list = list->next;
    }
}


void mark_meta(uint64_t temp) {
    size_t offset = (8 - (sizeof(meta) % 8)) % 8;
    meta* list = heap;
    if ((temp >= ((uintptr_t)heap + sizeof(meta) + offset )) && temp < ((uintptr_t)heap + 4096)) {
        while (list != NULL) {
            uintptr_t start = (uintptr_t)list + sizeof(meta) + offset;
           uintptr_t end   = start + list->size;
            if ((temp >= start) && (temp < end)) {
                list->reachable = true;
                break;
            }
            list = list->next;
        }
    }
    //This function goes to every block in the heap linked list and checks if the temp value lies in the the address range of any meta block
}



