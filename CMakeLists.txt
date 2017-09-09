cmake_minimum_required(VERSION 3.2)
project(dragon_vm)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if (COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif (COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

try_run(IS_LITTLE_ENDIAN COMPILE_RESULT ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/build-test/check-le-or-be.c)
if (IS_LITTLE_ENDIAN)
    add_definitions(-DDVM_LITTLE_ENDIAN=1)
endif ()

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

set(SOURCE_FILES
        include/*
        src/*)

# Pure Garbage Collector
add_library(dragon_vm_gc SHARED
        src/gc/pure_gc.cpp
        include/gc/pure_gc.hpp
        include/gc/gc_provider.hpp)

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
        include/core/interpreter/interpreter.hpp
        include/core/interpreter/opcodes.hpp
        include/core/exceptions.hpp
        include/core/errorcodes.hpp

        include/core/interpreter/vm_context.hpp
        include/core/interpreter/vm_register.hpp
        include/core/interpreter/vm_register_common.hpp
        include/core/interpreter/vm_register_visitor.hpp

        include/core/object/basic_classes.hpp
        include/core/object/method.hpp
        include/core/object/class.hpp
        include/core/object/slot.hpp

        include/core/dcx/dcx_file.hpp
        include/core/dcx/dcx_reader.hpp

        src/core/vm-main.cpp
        src/core/heap.cpp
        src/core/memory.cpp
        src/core/interpreter/interpreter.cpp
        src/core/config.cpp
        src/core/heap.cpp

        src/core/interpreter/vm_register.cpp
        src/core/interpreter/vm_register_visitor.cpp
        src/core/interpreter/vm_context.cpp

        src/core/object/class.cpp
        src/core/object/object.cpp
        src/core/object/basic_classes.cpp
        src/core/object/method.cpp
        src/core/object/abs_method.hpp
        src/core/object/abs_method.cpp
        src/core/object/foreign_method.cpp
        src/core/object/foreign_method.hpp
        src/core/object/dvm_method.cpp
        src/core/object/dvm_method.hpp

        src/core/dcx/dcx_reader.cpp
        include/core/dcx/byte_ordered_reader.hpp
        )

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

add_executable(test-vm_register tests/test-vm_register.cpp)
target_link_libraries(test-vm_register dragon_vm_core)

add_executable(test-type tests/test-type.cpp)
target_link_libraries(test-type dragon_vm_core)

add_executable(test-object tests/test-object.cpp)
target_link_libraries(test-object dragon_vm_core)

add_executable(test-method tests/test-method.cpp)
target_link_libraries(test-method dragon_vm_core)

add_executable(test-dcx tests/test-dcx.cpp)
target_link_libraries(test-dcx dragon_vm_core)

# Build tools
add_executable(generate-empty-dcx build-test/generate-empty-dcx.cpp)

# Experimental test cases
add_executable(exp-threaded-interpreter
        experimental-tests/threaded-interpretation/opcodes.h
        experimental-tests/threaded-interpretation/threaded-interpreter.h
        experimental-tests/threaded-interpretation/threaded-interpreter.c
        experimental-tests/threaded-interpretation/benchmark_main.c
        experimental-tests/threaded-interpretation/decode-dispatch-interpreter.c
        experimental-tests/threaded-interpretation/decode-dispatch-interpreter.h)
