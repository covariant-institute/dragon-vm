//
// Created by kiva on 2017/8/30.
//
#include <cstdio>
#include <core/object/object.hpp>

#define P(T) \
    printf("sizeof(" #T ") = %zd\n", sizeof(T))

int main() {
    using namespace dvm::core;
    P(Int8);
    P(UInt8);
    P(Int16);
    P(UInt16);
    P(Int32);
    P(UInt32);
    P(Int64);
    P(UInt64);
    P(Byte);
    P(Bool);
    P(Float);
    P(Double);
    P(dvm::core::object::Object);
}
