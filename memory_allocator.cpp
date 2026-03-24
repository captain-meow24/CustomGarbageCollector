//
// Created by kanishka on 23/2/26.
//
#include "memory_allocator.h"

meta* heap = NULL;

meta* find_free(size_t req_size, meta* start) {
    meta* temp = start;
    while (temp != NULL) {
        if (temp->free && temp->size >= (req_size + sizeof(meta))){
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}


void createMeta(size_t req_size, meta* current) {

    meta* newm = reinterpret_cast<meta*>((char*)current + req_size + sizeof(meta));   //making a new meta block where the requested user memory ends
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

void* alloc(size_t req_size) {

    size_t offset = 8 - (sizeof(meta)%8);   //size of the padding between meta chunk and user memory, to align start and end of the user memory by 8 bytes
    //this is done so that we can each word can be scanned and checked for pointer to heap, hence implementing mark and sweep for garbage collection
    if (offset == 8) {
        offset = 0;
    }

    if (req_size%8) {
        size_t alignm = req_size%8;  //aligning user memory to 8 bytes
        alignm = 8 - alignm;
        req_size += alignm;
    }
    if (heap == NULL) {
        heap = reinterpret_cast<meta*>(mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
        //we are passing address, size, permissions, private/shared flag, file descriptor and offset.
        heap->prev = NULL;
        heap->free = true;
        heap->size = 4096 - sizeof(meta);
        heap->next = NULL;
        createMeta(req_size, heap);
        return (char*)(heap+1)+ offset;
    }
    meta* free_space = find_free(req_size, heap);
    if (free_space) {
        createMeta(req_size, free_space);
        return free_space +1 +offset;
    }
    return NULL;
}

void free_memory(meta* garbage) {
    if (garbage->prev && garbage->next && garbage->prev->free && garbage->next->free) {
        garbage->prev->size += sizeof(meta) + sizeof(meta) + garbage->size + garbage->next->size;
        garbage->prev->next = garbage->next->next;
        if (garbage->next->next) {
            garbage->next->next->prev = garbage->prev;
        }
    }
    else if (garbage->prev && garbage->prev->free) {
        garbage->prev->size += garbage->size + sizeof(meta);
        garbage->prev->next = garbage->next;
        if (garbage->next) {
            garbage->next->prev = garbage->prev;
        }
    }
    else if (garbage->next && garbage->next->free) {
        garbage->next->prev = garbage->prev;
        garbage->next->size += sizeof(meta) + garbage->size;
        if (garbage->prev) {
            garbage->prev->next = garbage->next;
        }
    }
    else {
        garbage->free = true;
    }
}
