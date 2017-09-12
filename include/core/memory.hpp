#pragma once

#include <core/type.hpp>

namespace dvm {
    namespace core {
        struct mem_block_info final {
            type_identifier type_id;
            Addr data_size;
            Byte reachable;
        };

        void *dvm_malloc(SizeT size);

        void dvm_free(void *ptr);

        template <typename T, typename...ArgsT>
        void dvm_construct_object(T *ptr, ArgsT &&...args) {
            if (ptr != nullptr) {
                ::new(ptr) T(std::forward<ArgsT>(args)...);
            }
        }

        template <typename T>
        void dvm_construct_object_array(T *elements, const T *array, SizeT length) {
            for (int i = 0; i < length; ++i) {
                dvm_construct_object(elements + i, array[i]);
            }
        }

        template <typename T>
        void dvm_destroy_object(T *ptr) {
            if (ptr != nullptr) {
                ptr->~T();
            }
        }
    }
}