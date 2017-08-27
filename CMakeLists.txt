cmake_minimum_required(VERSION 3.8)
project(dragon_vm)

set(CMAKE_CXX_STANDARD 14)

include_directories(headers)


add_library(dragon_vm_core SHARED
        src/core/vm-main.cpp
        headers/config.hpp
        headers/stack.hpp
        headers/type.hpp
        headers/utils.hpp headers/dvm.hpp)

add_executable(dragon_vm_cmdline src/main.cpp)

add_executable(test-stack test/test-stack.cpp)

target_link_libraries(dragon_vm_cmdline dragon_vm_core)
