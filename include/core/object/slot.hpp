//
// Created by kiva on 2017/8/31.
//
#pragma once

#include <core/type.hpp>
#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>

namespace dvm {
    namespace core {
        namespace object {
            struct Object;

            struct Slot {
                union {
                    Int32 i32;
                    Int64 i64;

                    Float f32;
                    Double f64;

                    Object *object;
                };

#define SETTER_GENERATOR(TYPE, MEMBER) \
                void set_##MEMBER(TYPE value) { \
                    (MEMBER) = (TYPE) (value); \
                }

                SETTER_GENERATOR(Int32, i32)

                SETTER_GENERATOR(Int64, i64)

                SETTER_GENERATOR(UInt32, i32)

                SETTER_GENERATOR(UInt64, i64)

                SETTER_GENERATOR(Float, f32)

                SETTER_GENERATOR(Double, f64)

                SETTER_GENERATOR(Object*, object)

#undef SETTER_GENERATOR
            };
        }
    }
}
