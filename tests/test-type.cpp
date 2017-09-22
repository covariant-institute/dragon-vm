//
// Created by kiva on 2017/8/30.
//
#include <cstdio>
#include <core/object/object.hpp>
#include <cassert>

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

    union {
        Float f;
        unsigned char bits[sizeof(Float)];
    } u{ };

    u.bits[0] = 0xffffffc3;
    u.bits[1] = 0xfffffff5;
    u.bits[2] = 0x48;
    u.bits[3] = 0x40;
    assert(u.f == 3.14f);

    u.bits[0] = static_cast<unsigned char>(-61);
    u.bits[1] = static_cast<unsigned char>(-11);
    u.bits[2] = 72;
    u.bits[3] = 64;
    assert(u.f == 3.14f);
}
