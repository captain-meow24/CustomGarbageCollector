#include "memory_allocator_garbage_collector.h"

meta *heap = NULL;
char *stack_high = nullptr;
void *current_top = nullptr;

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

void *alloc(size_t req_size) {
    size_t offset = 8 - (sizeof(meta) % 8);
    if (offset == 8) {
        offset = 0;
    }

    if (req_size % 8) {
        size_t alignm = req_size % 8;
        alignm = 8 - alignm;
        req_size += alignm;
    }
    if (heap == NULL) {
        heap = reinterpret_cast<meta *>(mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
        heap->prev = NULL;
        heap->free = true;
        heap->size = 4096 - sizeof(meta);
        heap->next = NULL;
        createMeta(req_size, heap);
        return reinterpret_cast<char*>(heap + 1) + offset;
    }
    meta *free_space = find_free(req_size, heap);
    if (free_space) {
        createMeta(req_size, free_space);
        return free_space + 1 + offset;
    }
    get_stack();
    scan_stack();
    scan_heap();
    sweep();
    reset_reachable();
    meta *new_free = find_free(req_size,heap);
    if (new_free) {
        createMeta(req_size, new_free);
        return new_free + 1 + offset;
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
    char *stack_low;
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

void scan_heap() {
    size_t offset = (8 - (sizeof(meta) % 8)) % 8;
    meta *list = heap;
    while (list != NULL) {
        if (list->reachable) {
            char *start = (char *) list + offset + sizeof(meta);
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
    size_t offset = (8 - (sizeof(meta) % 8)) % 8;
    meta* list = heap;
    if ((temp >= ((uintptr_t)heap + sizeof(meta) + offset)) && temp < ((uintptr_t)heap + 4096)) {
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
}

void sweep() {
    meta* current = heap;
    while (current != NULL) {
        if (current->reachable == false && current->free==false) {
            free_memory(current);
        }
        current = current->next;
    }
}

void reset_reachable() {
    meta* temp = heap;
    while (temp!=NULL) {
        temp->reachable = false;
        temp=temp->next;
    }
}