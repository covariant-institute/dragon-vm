cmake_minimum_required(VERSION 3.8)
project(dragon_vm)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/main.cpp headers/config.hpp headers/stack.hpp headers/type.hpp headers/utils.hpp)

add_executable(dragon_vm ${SOURCE_FILES})

include_directories(headers)
