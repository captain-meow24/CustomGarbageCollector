#include "memory_allocator_garbage_collector.h"

void example() {
    int* b = (int*)alloc(1000 * sizeof(int));
    *b = 12;
}

int main() {
    example();
    int* b = (int*)alloc(100);
    *b = 12;
    std::cout<<*b<<std::endl;
    std::cout<<b;
    return 0;
}