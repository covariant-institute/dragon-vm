#pragma once

#include <core/interpreter/vm_register.hpp>
namespace dvm {
    namespace core {
        namespace vm {
            class vm_context {
            private:


            public:
                vm_context() = delete;

                vm_context(const vm_context &) = delete;

                vm_context(ULong stack_size, ULong heap_size);

                ~vm_context();
            };
        }
    }
}

