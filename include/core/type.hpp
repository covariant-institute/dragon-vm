#pragma once

#include <cstdint>

namespace dvm {
    namespace core {
        using Char = int8_t;
        using Int = int32_t;
        using Long = int64_t;

        using UChar = uint8_t;
        using UInt = uint32_t;
        using ULong = uint64_t;

        using Byte = UChar;
        using Link = Byte *;
        using Bool = Byte;

        using Float = float;
        using Double = double;

        /**
         * 我们用了 ULong 作为 stack 和 heap 大小的类型
         * 与虚拟机中寻址保持一致，Addr 也用 ULong
         */
        using Addr = ULong;

        constexpr Bool True = static_cast<Bool>(true);
        constexpr Bool False = static_cast<Bool>(false);

        enum class type_identifier : UInt {
            TYPE_ID_UNSPECIFIC = 0,
            TYPE_ID_CHAR = 1,
            TYPE_ID_INT = 2,
            TYPE_ID_LONG = 3,
            TYPE_ID_UCHAR = 4,
            TYPE_ID_UINT = 5,
            TYPE_ID_ULONG = 6,
            TYPE_ID_FLOAT = 7,
            TYPE_ID_DOUBLE = 8,
            TYPE_ID_LINK = 9,
        };

        template <typename T>
        struct type_id_converter {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_UNSPECIFIC;
            }
        };

        template <>
        struct type_id_converter<Char> {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_CHAR;
            }
        };

        template <>
        struct type_id_converter<Int> {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_INT;
            }
        };

        template <>
        struct type_id_converter<Long> {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_LONG;
            }
        };

        template <>
        struct type_id_converter<UChar> {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_UCHAR;
            }
        };

        template <>
        struct type_id_converter<UInt> {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_UINT;
            }
        };

        template <>
        struct type_id_converter<ULong> {
            constexpr static type_identifier get_type_id() {
                return type_identifier::TYPE_ID_ULONG;
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
