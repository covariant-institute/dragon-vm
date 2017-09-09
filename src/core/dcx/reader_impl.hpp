//
// Created by kiva on 2017/9/8.
//

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
            };

            template <>
            struct little_endian<Int32> {
                static Int32 get_value(Int32 value) {
                    return ((value & 0x000000FF) << 24) |
                           ((value & 0x0000FF00) << 8) |
                           ((value & 0x00FF0000) >> 8) |
                           ((value & 0xFF000000) >> 24);
                }
            };

            template <>
            struct little_endian<UInt32> {
                static Int32 get_value(UInt32 value) {
                    return little_endian<Int32>::get_value(static_cast<Int32>(value));
                }
            };
#endif

            struct ByteOrderedReader {
                template <typename T>
                static inline T read_typed_data(FILE *file) {
                    T r;
                    fread(reinterpret_cast<void *>(&r), sizeof(T), 1, file);

#ifndef DVM_LITTLE_ENDIAN
                    r = little_endian<T>::get_value(r);
#endif
                    return r;
                }
            };
        }
    }
}
