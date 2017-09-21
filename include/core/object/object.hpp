//
// Created by kiva on 2017/8/31.
//
#pragma once

#include <core/type.hpp>
#include <core/object/slot.hpp>
#include <core/object/class.hpp>
#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>
#include <core/object/type_id_converter.hpp>

namespace dvm {
    namespace core {
        namespace object {
            struct Object {
                const Class *prototype;

                /* Keep it last */
                Slot slots[0];

                inline bool is_null() const {
                    return prototype == nullptr;
                }

                static Object *null_object();
            };

            inline void ensure_object_valid(Object *object) {
                if (object == nullptr) {
                    throw dvm::core::Exception(DVM_INVALID_OBJECT_MEMORY);
                }
            }
        }
    }
}
