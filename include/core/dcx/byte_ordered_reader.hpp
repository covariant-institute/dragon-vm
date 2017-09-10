//
// Created by kiva on 2017/9/8.
//
#pragma once

#include <core/type.hpp>
#include <cstdio>

namespace dvm {
    namespace core {
        namespace dcx {
#ifndef DVM_LITTLE_ENDIAN

            template <typename T>
            struct little_endian {
                static T get_value(T value) {
                    return 0;
                }

                static bool is_available() {
                    return false;
                }
            };

            template <>
            struct little_endian<Int32> {
                static Int32 get_value(Int32 value) {
                    return ((value & 0x000000FF) << 24) |
                           ((value & 0x0000FF00) << 8) |
                           ((value & 0x00FF0000) >> 8) |
                           ((value & 0xFF000000) >> 24);
                }

                static bool is_available() {
                    return true;
                }
            };

            template <>
            struct little_endian<UInt32> {
                static Int32 get_value(UInt32 value) {
                    return little_endian<Int32>::get_value(static_cast<Int32>(value));
                }

                static bool is_available() {
                    return true;
                }
            };
#endif

            struct ByteOrderedReader {
                template <typename T>
                static inline bool read(FILE *file, T *destination) {
                    size_t r = fread(reinterpret_cast<void *>(destination), sizeof(T), 1, file);

#ifndef DVM_LITTLE_ENDIAN
                    if (r == 1 && little_endian<T>::is_available()) {
                        *destination = little_endian<T>::get_value(*destination);
                    }
#endif
                    return r == 1;
                }

                static inline bool read_bytes(FILE *file, Byte *destination, SizeT length) {
                    return fread(reinterpret_cast<void *>(destination), sizeof(Byte), length, file) == length;
                }
            };
        }
    }
}
