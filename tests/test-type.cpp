//
// Created by kiva on 2017/8/30.
//
#include <cstdio>
#include <core/type.hpp>

#define print_size(T) \
    printf("sizeof(" #T ") = %zd\n", sizeof(T))

int main() {
    using namespace dvm::core;
    print_size(Char);
    print_size(UChar);
    print_size(Int);
    print_size(UInt);
    print_size(Long);
    print_size(ULong);
    print_size(Byte);
    print_size(Bool);
    print_size(Link);
    print_size(Float);
    print_size(Double);
}
