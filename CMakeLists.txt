cmake_minimum_required(VERSION 3.4)
project(dragon_vm)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

set(SOURCE_FILES
        include/*
        src/*)

# Pure Garbage Collector
add_library(dragon_vm_gc SHARED
        src/gc/gc.cpp
        include/gc/gc.hpp)

# Dragon VM Core
add_library(dragon_vm_core SHARED
        include/core/debug.hpp
        include/core/config.hpp
        include/core/memory.hpp
        include/core/stack.hpp
        include/core/heap.hpp
        include/core/type.hpp
        include/core/utils.hpp
        include/core/dvm.hpp
        include/core/interpreter.hpp
        include/core/opcodes.hpp
        include/core/exceptions.hpp
        include/core/errorcodes.hpp

        src/core/vm-main.cpp
        src/core/heap.cpp
        src/core/interpreter.cpp
        src/core/config.cpp
        src/core/heap.cpp)

# Command line executable, for debug
add_executable(dragon_vm_cmdline src/main.cpp)
target_link_libraries(dragon_vm_cmdline dragon_vm_core)

# Test cases
add_executable(test-stack tests/test-stack.cpp)
target_link_libraries(test-stack dragon_vm_core)

add_executable(test-heap tests/test-heap.cpp)
target_link_libraries(test-heap dragon_vm_core)

add_executable(test-gc tests/test-gc.cpp)
target_link_libraries(test-gc dragon_vm_gc)

add_executable(test-vm tests/test-vm.cpp)
target_link_libraries(test-vm dragon_vm_core)
