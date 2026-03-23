#include "memory_allocator.h"

int main() {
    int* a = (int*)alloc(sizeof(int));
    *a = 12;
    std::cout<<*a<<std::endl;
    std::cout<<a;
    return 0;
}