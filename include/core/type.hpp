#pragma once

#include <cstdint>

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
        using Link = Byte *;

        using Float = float;
        using Double = double;

        using SizeT = UInt64;

        /**
         * 我们用了 UInt64 作为 stack 和 heap 大小的类型
         * 与虚拟机中寻址保持一致，Addr 也用 UInt64
         */
        using Addr = UInt64;

        constexpr Bool True = static_cast<Bool>(true);
        constexpr Bool False = static_cast<Bool>(false);

        enum class type_identifier : UInt32 {
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
            TYPE_ID_LINK = 11,

            TYPE_ID_OBJECT = 12,
        };

        template <typename T>
        struct type_id_converter {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_UNSPECIFIC;
            }
        };

        template <>
        struct type_id_converter<Int8> {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_INT8;
            }
        };

        template <>
        struct type_id_converter<Int16> {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_INT16;
            }
        };

        template <>
        struct type_id_converter<Int32> {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_INT32;
            }
        };

        template <>
        struct type_id_converter<Int64> {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_INT64;
            }
        };

        template <>
        struct type_id_converter<UInt8> {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_UINT8;
            }
        };

        template <>
        struct type_id_converter<UInt16> {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_UINT16;
            }
        };

        template <>
        struct type_id_converter<UInt32> {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_UINT32;
            }
        };

        template <>
        struct type_id_converter<UInt64> {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_UINT64;
            }
        };

        template <>
        struct type_id_converter<Float> {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_FLOAT;
            }
        };

        template <>
        struct type_id_converter<Double> {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_DOUBLE;
            }
        };

        template <>
        struct type_id_converter<Link> {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_LINK;
            }
        };
    }
}
