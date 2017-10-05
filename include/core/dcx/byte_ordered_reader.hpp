//
// Created by kiva on 2017/9/8.
//
#pragma once

#include <core/type.hpp>
#include <cstdio>

namespace dvm {
    namespace core {
        namespace dcx {

            struct ByteOrderedReader {

                static Byte *to_little_endian(Byte *bytes, SizeT size) {
#ifndef DVM_LITTLE_ENDIAN

                    for (int i = 0; i < size / 2; ++i) {
                        std::swap(bytes[i], bytes[size - i - 1]);
                    }

#endif
                    return bytes;
                }

                template <typename T>
                static inline bool read(FILE *file, T *destination) {
                    size_t r = fread(reinterpret_cast<void *>(destination), sizeof(T), 1, file);
                    to_little_endian(destination, sizeof(T));

                    return r == 1;
                }

                static inline bool read_bytes(FILE *file, Byte *destination, SizeT length) {
                    return fread(reinterpret_cast<void *>(destination), sizeof(Byte), length, file) == length;
                }
            };
        }
    }
}
