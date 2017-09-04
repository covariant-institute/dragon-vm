//
// Created by kiva on 2017/8/31.
//
#pragma once

#include <core/type.hpp>
#include <core/object/slot.hpp>
#include <core/object/class.hpp>
#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>
#include <string>

namespace dvm {
    namespace core {
        namespace object {
            struct Object {
                const Class *prototype;

                /* Keep in last */
                Slot slots[0];

                static Object* create_object(const Class *prototype);
            };

            inline void ensure_valid(Object *object) {
                if (object == nullptr) {
                    throw dvm::core::exception(DVM_INVALID_OBJECT_MEMORY);
                }
            }

            inline bool is_uninitialized(Object *object) {
                ensure_valid(object);
                return object->prototype == nullptr;
            }
        }

        template <>
        struct type_id_converter<object::Object> {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_OBJECT;
            }
        };

        template <>
        struct type_id_converter<object::Object *> {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_OBJECT;
            }
        };
    }
}
