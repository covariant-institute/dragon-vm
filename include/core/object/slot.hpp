//
// Created by kiva on 2017/8/31.
//
#pragma once

#include <core/type.hpp>
#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>
#include <core/object/type_id_converter.hpp>
#include <core/utils.hpp>

namespace dvm {
    namespace core {
        namespace object {
            struct Object;

            struct Slot {
            private:
                type_identifier slot_type;

            public:
                union {
                    Int32 i32;
                    Int64 i64;

                    Float f32;
                    Double f64;

                    Object *object;
                } data;

                inline type_identifier get_type() const {
                    return slot_type;
                }

                inline void set_type(type_identifier new_type) {
                    if (new_type == type_identifier::TYPE_ID_UNSPECIFIC ||
                        (slot_type != type_identifier::TYPE_ID_UNSPECIFIC
                         && slot_type != new_type)) {
                        throw dvm::core::Exception(DVM_UNSATISFIED_SLOT_TYPE);
                    }
                    slot_type = new_type;
                }

                template <typename T>
                inline T get() const {
                    if (slot_type == type_identifier::TYPE_ID_UNSPECIFIC) {
                        throw dvm::core::Exception(DVM_UNSATISFIED_SLOT_TYPE);
                    }

                    return *reinterpret_cast<const T *>(&data);
                }

                template <typename T>
                inline void set(const T &t) {
                    set_type(type_id_converter<T>::get_type_id());
                    *reinterpret_cast<T *>(&data) = t;
                }

                template <typename T>
                inline void set_unchecked(const T &t) {
                    slot_type = type_identifier::TYPE_ID_UNSPECIFIC;
                    *reinterpret_cast<T *>(&data) = t;
                }
            };
        }
    }
}
