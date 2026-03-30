#include "alloc_function.h"

void example() {
    int* b = (int*)alloc(1000 * sizeof(int));
    *b = 12;
}

int main() {
    example();
    example();
    example();
    int* a = (int*)alloc(10);
    *a = 12;
    std::cout<<*a<<std::endl;
    std::cout<<a<<std::endl;
    return 0;
}