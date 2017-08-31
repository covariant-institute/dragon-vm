//
// Created by kiva on 2017/8/31.
//
#pragma once

#include <core/object/class.hpp>
#include <core/object/slot.hpp>
#include <string>

namespace dvm {
    namespace core {
        namespace object {
            struct Object {
                Class *prototype;
                int slot_count;
                Slot slots[0];
            };
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
