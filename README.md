# Custom Garbage Collector
This is a library that allocates memory on heap and collects unreachable memory or garbage automatically when we run out of memory. 
## Working of the memory allocator-
It allocates atleast 4KB memory on the heap using sbrk (a linux system call that increments the heap break), if the user asks for more than 4KB of memory, it allocates in multiples of 4KB that is sufficient for the requested size.
When we run out of this memory, it extends the heap further using system call.
## What is garbage?
Garbage is the memory that was allocated on the heap but is no longer reachable and hence useless. 
It is desirable to deallocate and reclaim the memory back into the heap so that there is enough space on the heap for future allocations.
For example-

```cpp
void example() {
    int* b = (int*)alloc(1000 * sizeof(int));
    *b = 12;
}

int main() {
    example();
    return 0;
}
```
The pointer created inside the example() function will go out of scope once the function call finishes but the memory allocated on the heap is there till the end of the process unless freed.
The garbage collector will identify this memory automatically and free it.

## Working-
I implemented mark and sweep algorithm for the garbage collector.
For this, we scan the process stack from current top to base address (highest address of the stack on x86) and check in chunks of 8-bytes one after another to see if any of them points to any allocated block on the heap.
We mark each block accessible that has any references to it and perform reachability analysis, that is, we check the data region of each block on the heap in chunks of 8-bytes to check if any of them reference to other blocks on the heap.
After we are done marking the blocks, we free all blocks of memory that are not reachable.

## Instructions to use-
Just copy paste this library into your code.
This library is linux dependent.
This is however, a conservative memory allocator and garbage collector which works on mark and sweep algorithm.
When a function finishes, the current top moves up in address (on x86) and all memory below is no longer part of the active stack frame, however, it is not erased, so when a new function is called, it reuses that memory.
As a result of this, the stack frame of the active function may contain the old pointers to the memory (that are garbage) in places where there is padding or offset, so while scanning the stack, these pointers may be interpreted as live references and may be marked as reachable sometimes.
Despite this, the garbage collector performs rather efficiently.
