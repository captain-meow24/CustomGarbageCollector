//
// Created by kanishka on 23/3/26.
//

#include "Garbage_collector.h"

void get_stack(void *stack_base_addr, size_t &size) {
    pthread_attr_t attr;
    pthread_getattr_np(pthread_self(), &attr);

    pthread_attr_getstack(&attr, &stack_base_addr, &size);
}
