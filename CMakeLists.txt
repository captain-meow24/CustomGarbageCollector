cmake_minimum_required(VERSION 3.31)
project(CustomGarbageCollector)

set(CMAKE_CXX_STANDARD 20)

add_executable(CustomGarbageCollector main.cpp
        memory_allocator_garbage_collector.h
        memory_allocator_garbage_collector.cpp
       )
