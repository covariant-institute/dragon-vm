//
// Created by kiva on 2017/8/30.
//
#include <cstdio>
#include <core/object/object.hpp>

#define print_size(T) \
    printf("sizeof(" #T ") = %zd\n", sizeof(T))

int main() {
    using namespace dvm::core;
    print_size(Int8);
    print_size(UInt8);
    print_size(Int16);
    print_size(UInt16);
    print_size(Int32);
    print_size(UInt32);
    print_size(Int64);
    print_size(UInt64);
    print_size(Byte);
    print_size(Bool);
    print_size(Link);
    print_size(Float);
    print_size(Double);
    print_size(dvm::core::object::Object);
}
