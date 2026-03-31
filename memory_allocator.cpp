//
// Created by kanishka on 27/3/26.
//
#include "memory_allocator.h"

char* heap_break = 0;

meta *heap = NULL;
char *stack_high = nullptr;
void *current_top = nullptr;


meta *find_free(size_t req_size, meta *start) {
    //finds free block of memory greater than or equal to the requested size
    meta *temp = start;
    while (temp != NULL) {
        if (temp->free && temp->size >= req_size) {
            return temp;
            //returns the meta block
        }
        temp = temp->next;
    }
    return NULL;
}

void createMeta(size_t req_size, meta *current) {
    if (current->size >= req_size + sizeof(meta) + 8) {
        //checks if the free memory is greater than or equal to requested size + enough memory to create meta data + 8 bytes buffer
        meta *newm = reinterpret_cast<meta *>((char *) current + req_size + sizeof(meta));
        //since the memory is enough, it creates a new block at distance after size of meta block + requested size
        //we are converting current to char for appropriate pointer arithematic
        newm->next = current->next;
        if (newm->next) newm->next->prev = newm;
        newm->free = true;
        newm->reachable = false;
        newm->size = current->size - req_size - sizeof(meta);
        newm->prev = current;
        current->next = newm;
    }
    //if the size is not enough for a new meta block then we just allocate that block of memory and not create new block
    current->size = req_size;
    current->free = false;
    current->reachable = false;
}

void *allocate(size_t req_size) {

    if (heap == NULL) {
        int size_flag = (req_size/4096) + 1;
        heap = reinterpret_cast<meta*>(sbrk(4096*size_flag));
        heap_break = (char*)heap + 4096*size_flag;
        heap->prev = NULL;
        heap->free = true;
        heap->size = 4096*size_flag - sizeof(meta);
        heap->next = NULL;
        createMeta(req_size, heap);
        return reinterpret_cast<char*>(heap + 1);
    }
    meta *free_space = find_free(req_size, heap);
    if (free_space) {
        createMeta(req_size, free_space);
        return free_space + 1 ;
    }
    return NULL;
}
void free_memory(meta *garbage) {
    //frees the allocated blocks and merges them
    garbage->free = true;
    if (garbage->prev && garbage->next && garbage->prev->free && garbage->next->free) {
        //when both neighbours are free, merge them into a single free block
        garbage->prev->size += sizeof(meta) + sizeof(meta) + garbage->size + garbage->next->size;
        garbage->prev->next = garbage->next->next;
        if (garbage->next->next) {
            //if the next of garbage's next exists, then change its prev to garbage's prev
            garbage->next->next->prev = garbage->prev;
        }
    } else if (garbage->prev && garbage->prev->free) {
        //if only prev of garbage is free, merge garbage into prev block
        garbage->prev->size += garbage->size + sizeof(meta);
        garbage->prev->next = garbage->next;
        if (garbage->next) {
            garbage->next->prev = garbage->prev;
        }
    } else if (garbage->next && garbage->next->free) {
        //if only garbage's next exists, merge them into freed garbage block
        if (garbage->next->next) {
            garbage->next->next->prev = garbage;
        }
        garbage->size += sizeof(meta) + garbage->next->size;
        garbage->next = garbage->next->next;
    }
}

void print_heap() {
    //this function iterates through the double linked list that is heap and calculates total allocated ans free memory ad prints it
    meta* temp = heap;
    size_t total_alloced = 0;
    size_t total_free = 0;
    while(temp !=NULL) {
        if (temp->free) {
            total_free += temp->size;
        }
        else {
            total_alloced += temp->size;
        }
        temp=temp->next;
    }
    std::cout<<"Total allocated memory: "<<total_alloced << ", Total free memory: "<<total_free<<std::endl;
}

