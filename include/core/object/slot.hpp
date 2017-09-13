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

                inline void ensure_type_valid(type_identifier current_type) {
                    // 如果这个 slot 没有被初始化，则以第一次初始化时的类型设定这个 slot 的类型
                    if (slot_type == type_identifier::TYPE_ID_UNSPECIFIC) {
                        this->slot_type = current_type;

                    } else if (current_type != slot_type) {
                        throw dvm::core::exception(DVM_UNSATISFIED_SLOT_TYPE);
                    }
                }

#define SETTER_GENERATOR(TYPE, MEMBER, TYPE_ID) \
                void set_##MEMBER(TYPE value) { \
                    ensure_type_valid(TYPE_ID); \
                    (MEMBER) = value; \
                }

                SETTER_GENERATOR(Int8, i8, type_identifier::TYPE_ID_INT8)

                SETTER_GENERATOR(Int16, i16, type_identifier::TYPE_ID_INT16)

                SETTER_GENERATOR(Int32, i32, type_identifier::TYPE_ID_INT32)

                SETTER_GENERATOR(Int64, i64, type_identifier::TYPE_ID_INT64)

                SETTER_GENERATOR(UInt8, u8, type_identifier::TYPE_ID_UINT8)

                SETTER_GENERATOR(UInt16, u16, type_identifier::TYPE_ID_UINT16)

                SETTER_GENERATOR(UInt32, u32, type_identifier::TYPE_ID_UINT32)

                SETTER_GENERATOR(UInt64, u64, type_identifier::TYPE_ID_UINT64)

                SETTER_GENERATOR(Float, f32, type_identifier::TYPE_ID_FLOAT)

                SETTER_GENERATOR(Double, f64, type_identifier::TYPE_ID_DOUBLE)

                SETTER_GENERATOR(Object*, object, type_identifier::TYPE_ID_OBJECT)

#undef SETTER_GENERATOR

                inline void set_byte(Byte value) {
                    set_u8(value);
                }

                inline void set_bool(Bool value) {
                    set_byte(value);
                }
            };
        }
    }
}
