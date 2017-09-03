//
// Created by kiva on 2017/8/31.
//
#pragma once

#include <core/type.hpp>

namespace dvm {
    namespace core {
        namespace object {
            struct Object;

            struct Slot {
                type_identifier slot_type;
                union {
                    Int8 i8;
                    Int16 i16;
                    Int32 i32;
                    Int64 i64;

                    UInt8 u8;
                    UInt16 u16;
                    UInt32 u32;
                    UInt64 u64;

                    Float f32;
                    Double f64;

                    Object *object;
                };
            };
        }
    }
}
