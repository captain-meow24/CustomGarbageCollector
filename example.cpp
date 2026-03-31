#include "alloc_function.h"

void example() {
    int* b = (int*)alloc(1500 * sizeof(int));
    *b = 12;
}

int main() {
    example();
    example();
    example();
    int* a = (int*)alloc(100);
    *a = 12;
    int* b = (int*)alloc(100);
    *b = 22;
    std::cout<<*a<<std::endl;
    std::cout<<a<<std::endl;
    std::cout<<*b<<std::endl;
    std::cout<<b<<std::endl;
    return 0;
}
