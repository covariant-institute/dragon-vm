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

include(CheckCCompilerFlag)
CHECK_C_COMPILER_FLAG("-std=c11" COMPILER_SUPPORTS_C11)
if (COMPILER_SUPPORTS_CXX11)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
else()
    message(STATUS "The compiler ${CMAKE_C_COMPILER} has no C11 support. Please use a different C compiler.")
endif()
set(CMAKE_C_STANDARD 11)

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
add_library(dvm-gc
        src/gc/pure_gc.cpp
        include/gc/pure_gc.hpp
        )

set(DVM_CORE_SOURCE
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
        include/core/runtime/context.hpp
        include/core/runtime/register.hpp
        include/core/runtime/register_common.hpp
        include/core/runtime/register_visitor.hpp
        include/core/runtime/thread.hpp
        include/core/runtime/dispatcher.hpp
        include/core/runtime/math.hpp
        include/core/runtime/jump.hpp
        include/core/runtime/dvm.hpp
        include/core/runtime/invoke.hpp
        include/core/runtime/throw.hpp

        include/core/object/method.hpp
        include/core/object/class.hpp
        include/core/object/slot.hpp
        include/core/object/type_id_converter.hpp
        include/core/object/array.hpp
        include/core/object/box.hpp
        include/core/object/reference.hpp

        include/core/dcx/dcx_file_info.hpp
        include/core/dcx/dcx_reader.hpp
        include/core/dcx/byte_ordered_reader.hpp
        include/core/dcx/dcx_file.hpp
        include/core/dcx/dcx_linker.hpp

        include/core/platform/dl_commom.hpp
        include/core/platform/dl.hpp

        include/abi/resolve.hpp

        src/core/vm_main.cpp
        src/core/heap.cpp
        src/core/stack.cpp
        src/core/memory.cpp
        src/core/config.cpp
        src/core/heap.cpp

        src/core/runtime/interpreter.cpp
        src/core/runtime/register.cpp
        src/core/runtime/register_visitor.cpp
        src/core/runtime/context.cpp
        src/core/runtime/thread.cpp
        src/core/runtime/dvm.cpp
        src/core/runtime/throw.cpp

        src/core/object/class.cpp
        src/core/object/object.cpp
        src/core/object/method.cpp
        src/core/object/dvm_method.cpp
        src/core/object/dvm_method.hpp
        src/core/object/native_method.hpp
        src/core/object/native_method.cpp
        src/core/object/array.cpp
        src/core/object/reference.cpp

        src/core/dcx/dcx_reader.cpp
        src/core/dcx/dcx_file.cpp
        src/core/dcx/dcx_linker.cpp

        src/abi/resolve.cpp

        ${DVM_PLATFORM_SRC})

# Dragon VM Core
add_library(dvm-core SHARED ${DVM_CORE_SOURCE})
add_library(dvm-core-static ${DVM_CORE_SOURCE})

target_link_libraries(dvm-core dvm-gc)
target_link_libraries(dvm-core-static dvm-gc)

# Command line executable, for debug
add_executable(dragon_vm_cmdline src/main.cpp)
target_link_libraries(dragon_vm_cmdline dvm-core)

# Test cases
add_executable(test-stack tests/test-stack.cpp)
target_link_libraries(test-stack dvm-core-static)

add_executable(test-gc tests/test-gc.cpp)
target_link_libraries(test-gc dvm-gc)

add_executable(test-register tests/test-register.cpp)
target_link_libraries(test-register dvm-core-static)

add_executable(test-type tests/test-type.cpp)
target_link_libraries(test-type dvm-core-static)

add_executable(test-object tests/test-object.cpp)
target_link_libraries(test-object dvm-core-static)

add_executable(test-method tests/test-method.cpp)
target_link_libraries(test-method dvm-core-static)

add_executable(test-dcx tests/test-dcx.cpp)
target_link_libraries(test-dcx dvm-core-static)

add_executable(test-interpreter tests/test-interpreter.cpp)
target_link_libraries(test-interpreter dvm-core-static)

add_executable(test-byte-reader tests/test-byte-reader.cpp)
target_link_libraries(test-byte-reader dvm-core-static)

add_executable(test-boxing tests/test-boxing.cpp)
target_link_libraries(test-boxing dvm-core-static)

add_executable(test-array tests/test-array.cpp)
target_link_libraries(test-array dvm-core-static)

add_executable(test-reference tests/test-reference.cpp)
target_link_libraries(test-reference dvm-core-static)

# Build tools
add_executable(generate-empty-dcx tools/generate-empty-dcx.cpp)
add_executable(generate-opcode-table tools/generate-opcode-table.cpp)
add_executable(generate-opcode-impl tools/generate-opcode-impl.cpp)
add_executable(generate-libdvm-hpp tools/generate-libdvm-h.cpp)

add_executable(dvm-exec tools/dvm-exec.cpp)
target_link_libraries(dvm-exec dvm-core-static)

# Experimental test cases
add_executable(exp-threaded-interpreter
        experimental/threaded-interpretation/opcodes.h
        experimental/threaded-interpretation/threaded-interpreter.h
        experimental/threaded-interpretation/threaded-interpreter.c
        experimental/threaded-interpretation/benchmark_main.c
        experimental/threaded-interpretation/decode-dispatch-interpreter.c
        experimental/threaded-interpretation/decode-dispatch-interpreter.h)

add_executable(exp-universal-memory
        experimental/universal-memory/test.cpp)

add_executable(exp-calc-compiler
        experimental/calc-compiler/y.tab.c
        experimental/calc-compiler/y.tab.h
        experimental/calc-compiler/lex.yy.c
        experimental/calc-compiler/main.c)

if (WIN32)
elseif (UNIX)
    target_link_libraries(dvm-core -ldl)
    target_link_libraries(dvm-core-static -ldl)
endif()
