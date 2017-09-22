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
set(CMAKE_CXX_STANDARD 11)
include_directories(include)

try_run(IS_LITTLE_ENDIAN COMPILE_RESULT ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/tools/check-le-or-be.c)
if (IS_LITTLE_ENDIAN)
    add_definitions(-DDVM_LITTLE_ENDIAN=1)
endif ()

try_run(IS_THREADED COMPILE_RESULT ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/tools/check-threaded.cpp)
if (IS_THREADED)
    add_definitions(-DDVM_INTERPRETATION_THREADED)
endif ()

if (WIN32)
    add_definitions(-DDVM_PLATFORM_WINDOWS)
    aux_source_directory(src/core/platform/windows DVM_PLATFORM_SRC)
elseif (UNIX)
    add_definitions(-DDVM_PLATFORM_UNIX)
    aux_source_directory(src/core/platform/unix DVM_PLATFORM_SRC)
else()
    message(STATUS "Cannot detect current system, disabling platform dependent features.")
endif()

# Pure Garbage Collector
add_library(dragon_vm_gc
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
        include/core/exceptions.hpp
        include/core/errorcodes.hpp

        include/core/runtime/interpreter.hpp
        include/core/runtime/opcodes.hpp
        include/core/runtime/vm_context.hpp
        include/core/runtime/vm_register.hpp
        include/core/runtime/vm_register_common.hpp
        include/core/runtime/vm_register_visitor.hpp
        include/core/runtime/thread.hpp

        include/core/object/method.hpp
        include/core/object/class.hpp
        include/core/object/slot.hpp
        include/core/object/type_id_converter.hpp

        include/core/dcx/dcx_file_info.hpp
        include/core/dcx/dcx_reader.hpp
        include/core/dcx/byte_ordered_reader.hpp
        include/core/dcx/dcx_file.hpp
        include/core/dcx/dcx_linker.hpp

        include/core/platform/dl_commom.hpp
        include/core/platform/dl.hpp

        src/core/vm-main.cpp
        src/core/heap.cpp
        src/core/stack.cpp
        src/core/memory.cpp
        src/core/config.cpp
        src/core/heap.cpp

        src/core/runtime/interpreter.cpp
        src/core/runtime/vm_register.cpp
        src/core/runtime/vm_register_visitor.cpp
        src/core/runtime/vm_context.cpp
        src/core/runtime/thread.cpp

        src/core/object/class.cpp
        src/core/object/object.cpp
        src/core/object/method.cpp
        src/core/object/abs_method.hpp
        src/core/object/abs_method.cpp
        src/core/object/dvm_method.cpp
        src/core/object/dvm_method.hpp
        src/core/object/native_method.hpp
        src/core/object/native_method.cpp

        src/core/dcx/dcx_reader.cpp
        src/core/dcx/dcx_file.cpp
        src/core/dcx/dcx_linker.cpp

        ${DVM_PLATFORM_SRC} include/core/runtime/utils.hpp)

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

add_executable(test-interpreter tests/test-interpreter.cpp)
target_link_libraries(test-interpreter dragon_vm_core)

# Build tools
add_executable(generate-empty-dcx tools/generate-empty-dcx.cpp)
add_executable(generate-opcode-table tools/generate-opcode-table.cpp)
add_executable(generate-opcode-impl tools/generate-opcode-impl.cpp)

# Experimental test cases
add_executable(exp-threaded-interpreter
        experimental-tests/threaded-interpretation/opcodes.h
        experimental-tests/threaded-interpretation/threaded-interpreter.h
        experimental-tests/threaded-interpretation/threaded-interpreter.c
        experimental-tests/threaded-interpretation/benchmark_main.c
        experimental-tests/threaded-interpretation/decode-dispatch-interpreter.c
        experimental-tests/threaded-interpretation/decode-dispatch-interpreter.h)

add_executable(exp-universal-memory
        experimental-tests/universal-memory/test.cpp)

if (WIN32)
elseif (UNIX)
    target_link_libraries(dragon_vm_core -ldl)
endif()
