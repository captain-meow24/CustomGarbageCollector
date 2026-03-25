#include "memory_allocator_garbage_collector.h"

void example() {
    int* b = (int*)alloc(1000 * sizeof(int));
    *b = 12;
}

int main() {
    example();
    int* a = (int*)alloc(1000 * sizeof(int));
    *a = 12;
    std::cout<<*a<<std::endl;
    std::cout<<a;
    return 0;
}