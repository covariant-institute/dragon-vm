//
// Created by kiva on 2017/9/17.
//
#pragma once

#include <core/type.hpp>

namespace dvm {
    namespace core {
        namespace object {
            class Object;
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
