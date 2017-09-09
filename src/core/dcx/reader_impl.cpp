//
// Created by kiva on 2017/9/8.
//

#include "reader_impl.hpp"

namespace dvm {
    namespace core {
        namespace dcx {
#ifndef DVM_LITTLE_ENDIAN
            static UInt32 big_to_little_u32(UInt32 value) {
                return ((value & 0x000000FF) << 24) |
                       ((value & 0x0000FF00) << 8) |
                       ((value & 0x00FF0000) >> 8) |
                       ((value & 0xFF000000) >> 24);
            }

            static UInt32 big_to_little_i32(Int32 value) {
                return big_to_little_u32(static_cast<UInt32>(value));
            }
#endif

#define COMMON_READER_GENERATOR(TYPE, TYPE_SHORT) \
            static TYPE read_##TYPE_SHORT##_common(FILE *file) { \
                TYPE r; \
                fread(reinterpret_cast<void *>(&r), sizeof(TYPE), 1, file); \
                return r; \
            }

            COMMON_READER_GENERATOR(Int32, i32)

            COMMON_READER_GENERATOR(UInt32, u32)

#undef COMMON_READER_GENERATOR


#ifdef DVM_LITTLE_ENDIAN
#define READER_GENERATOR(TYPE, TYPE_SHORT) \
            TYPE read_##TYPE_SHORT(FILE *file) { \
                return read_##TYPE_SHORT##_common(file); \
            }
#else
#define READER_GENERATOR(TYPE, TYPE_SHORT) \
            TYPE read_##TYPE_SHORT(FILE *file) { \
                TYPE r = read_##TYPE_SHORT##_common(file); \
                r = big_to_endian_##TYPE_SHORT(r); \
                return r; \
            }
#endif

            READER_GENERATOR(UInt32, u32)

            READER_GENERATOR(Int32, i32)

#undef READER_GENERATOR
        }
    }
}
