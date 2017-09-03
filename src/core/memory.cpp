//
// Created by kiva on 2017/9/1.
//

#include <core/memory.hpp>
#include <cstdlib>
#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>


namespace dvm {
    namespace core {
        void* dvm_malloc(SizeT size) {
            void *memory = ::malloc(static_cast<size_t>(size));
            if (memory == nullptr) {
                throw dvm::core::exception(DVM_BAD_ALLOC);
            }
            return memory;
        }
    }
}

