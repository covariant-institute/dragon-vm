#pragma once

#include <cstdint>
#include <string>

namespace dvm {
    namespace core {
        using Int8 = int8_t;
        using Int16 = int16_t;
        using Int32 = int32_t;
        using Int64 = int64_t;

        using UInt8 = uint8_t;
        using UInt16 = uint16_t;
        using UInt32 = uint32_t;
        using UInt64 = uint64_t;

        using Byte = UInt8;
        using Bool = Byte;

        using Float = float;
        using Double = double;

        using SizeT = UInt64;

        constexpr Bool True = static_cast<Bool>(true);
        constexpr Bool False = static_cast<Bool>(false);

        enum class TypeIdentifier : UInt32 {
            TYPE_ID_UNSPECIFIC = 0,
            TYPE_ID_INT8 = 1,
            TYPE_ID_INT16 = 2,
            TYPE_ID_INT32 = 3,
            TYPE_ID_INT64 = 4,

            TYPE_ID_UINT8 = 5,
            TYPE_ID_UINT16 = 6,
            TYPE_ID_UINT32 = 7,
            TYPE_ID_UINT64 = 8,

            TYPE_ID_FLOAT = 9,
            TYPE_ID_DOUBLE = 10,

            TYPE_ID_OBJECT = 11,
            TYPE_ID_ARRAY = 12,
        };

        template <typename T>
        struct TypeIdConverter {
            constexpr static TypeIdentifier get_type_id() {
                return TypeIdentifier::TYPE_ID_UNSPECIFIC;
            }
        };

        template <>
        struct TypeIdConverter<Int8> {
            constexpr static TypeIdentifier get_type_id() {
                return TypeIdentifier::TYPE_ID_INT8;
            }
        };

        template <>
        struct TypeIdConverter<Int16> {
            constexpr static TypeIdentifier get_type_id() {
                return TypeIdentifier::TYPE_ID_INT16;
            }
        };

        template <>
        struct TypeIdConverter<Int32> {
            constexpr static TypeIdentifier get_type_id() {
                return TypeIdentifier::TYPE_ID_INT32;
            }
        };

        template <>
        struct TypeIdConverter<Int64> {
            constexpr static TypeIdentifier get_type_id() {
                return TypeIdentifier::TYPE_ID_INT64;
            }
        };

        template <>
        struct TypeIdConverter<UInt8> {
            constexpr static TypeIdentifier get_type_id() {
                return TypeIdentifier::TYPE_ID_UINT8;
            }
        };

        template <>
        struct TypeIdConverter<UInt16> {
            constexpr static TypeIdentifier get_type_id() {
                return TypeIdentifier::TYPE_ID_UINT16;
            }
        };

        template <>
        struct TypeIdConverter<UInt32> {
            constexpr static TypeIdentifier get_type_id() {
                return TypeIdentifier::TYPE_ID_UINT32;
            }
        };

        template <>
        struct TypeIdConverter<UInt64> {
            constexpr static TypeIdentifier get_type_id() {
                return TypeIdentifier::TYPE_ID_UINT64;
            }
        };

        template <>
        struct TypeIdConverter<Float> {
            constexpr static TypeIdentifier get_type_id() {
                return TypeIdentifier::TYPE_ID_FLOAT;
            }
        };

        template <>
        struct TypeIdConverter<Double> {
            constexpr static TypeIdentifier get_type_id() {
                return TypeIdentifier::TYPE_ID_DOUBLE;
            }
        };
    }
}
