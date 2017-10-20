//
// Created by kiva on 2017/9/17.
//
#pragma once

#include <core/type.hpp>

namespace dvm {
    namespace core {
        namespace object {
            class Object;

            class Array;
        }

        template <>
        struct TypeIdConverter<object::Object> {
            constexpr static TypeIdentifier get_type_id() {
                return TypeIdentifier::TYPE_ID_OBJECT;
            }
        };

        template <>
        struct TypeIdConverter<object::Object *> {
            constexpr static TypeIdentifier get_type_id() {
                return TypeIdentifier::TYPE_ID_OBJECT;
            }
        };

        template <>
        struct TypeIdConverter<object::Array> {
            constexpr static TypeIdentifier get_type_id() {
                return TypeIdentifier::TYPE_ID_ARRAY;
            }
        };

        template <>
        struct TypeIdConverter<object::Array *> {
            constexpr static TypeIdentifier get_type_id() {
                return TypeIdentifier::TYPE_ID_ARRAY;
            }
        };
    }
}
