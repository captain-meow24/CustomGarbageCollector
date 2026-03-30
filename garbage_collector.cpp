//
// Created by kanishka on 27/3/26.
//
#include "garbage_collector.h"

void get_stack() {
    size_t size;
    char *stack_low;    //lowest address
    pthread_attr_t attr;
    pthread_getattr_np(pthread_self(), &attr);
    pthread_attr_getstack(&attr, (void **) &stack_low, &size);
    stack_high = stack_low + size;
    pthread_attr_destroy(&attr);
    asm volatile("mov %%rsp, %0" : "=r"(current_top));
}

void scan_stack() {
    char *temp = (char *) current_top;
    while (temp < stack_high) {
        uint64_t value = *(uint64_t *) temp;
        mark_meta(value);
        temp += 8;
    }
}

void scan_heap(meta* root) {
    meta* list = root;

    while (list != NULL) {
        if (list->reachable) {
            char *start = (char *) list + sizeof(meta);
            char *end = start + list->size;

            for (char *curr = start; curr < end; curr += 8) {
                uint64_t val = *(uint64_t *) curr;
                mark_meta(val);
            }
        }
        list = list->next;
    }
}

void mark_meta(uint64_t temp) {
    meta* list = heap;
    if ((temp >= ((uintptr_t)heap + sizeof(meta) )) && temp < ((uintptr_t)heap + 4096* syscall_flag)) {
        while (list != NULL ) {
            uintptr_t start = (uintptr_t)list + sizeof(meta);
            uintptr_t end = start + list->size;
            if ((temp >= start) && (temp < end) && list->reachable == false) {
                list->reachable = true;
                scan_block(list);
                break;
            }
            list = list->next;
        }
    }
}

void sweep() {
    meta* current = heap;
    while (current != NULL) {
        meta* next = current->next;
        if (!current->reachable && !current->free) {
            free_memory(current);
        }
        current = next;
    }
}

void reset_reachable() {
    meta* temp = heap;
    while (temp!=NULL) {
        temp->reachable = false;
        temp=temp->next;
    }
}

void scan_block(meta* block) {
    char* start = (char*)block + sizeof(meta);
    char* end = start + block->size;

    for (char* curr = start; curr < end; curr += 8) {
        uint64_t val = *(uint64_t*)curr;
        mark_meta(val);
    }
}