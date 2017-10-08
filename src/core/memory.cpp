//
// Created by kiva on 2017/9/1.
//

#include <core/memory.hpp>
#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>
#include <cstring>
#include <cstdlib>


namespace dvm {
    namespace core {
        void* dvm_malloc(SizeT size) {
            void *memory = ::malloc(static_cast<size_t>(size));
            if (memory == nullptr) {
                throw dvm::core::Exception(DVM_BAD_ALLOC);
            }
            memset(memory, '\0', size);
            return memory;
        }

        void dvm_free(void *ptr) {
            if (ptr != nullptr) {
                ::free(ptr);
            }
        }
    }
}

