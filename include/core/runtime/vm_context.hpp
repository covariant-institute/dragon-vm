#pragma once

#include <core/runtime/vm_register.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            // TODO VM Executing Context
            class vm_context {
            private:

            public:
                vm_context() = delete;

                vm_context(const vm_context &) = delete;

                vm_context(UInt64 stack_size, UInt64 heap_size);

                ~vm_context() = default;
            };
        }
    }
}

