cmake_minimum_required(VERSION 3.8)
project(dragon_vm)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)


# Pure Garbage Collector
add_library(dragon_vm_gc SHARED
        src/gc/gc.cpp
        include/gc/gc.hpp)

# Dragon VM Core
add_library(dragon_vm_core SHARED
        src/core/vm-main.cpp
        src/core/interpreter.cpp

        include/core/config.hpp
        include/core/stack.hpp
        include/core/type.hpp
        include/core/utils.hpp
        include/core/dvm.hpp
        include/core/interpreter.hpp include/core/opcodes.hpp)

target_link_libraries(dragon_vm_core dragon_vm_gc)


# Command line executable, for debug
add_executable(dragon_vm_cmdline src/main.cpp)
target_link_libraries(dragon_vm_cmdline dragon_vm_core)

# Test cases
add_executable(test-stack tests/test-stack.cpp)
target_link_libraries(test-stack dragon_vm_core)

add_executable(test-gc tests/test-gc.cpp)
target_link_libraries(test-gc dragon_vm_gc)

add_executable(test-vm tests/test-vm.cpp)
target_link_libraries(test-vm dragon_vm_core)
