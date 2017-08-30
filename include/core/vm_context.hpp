#pragma once

namespace dvm {
    namespace core {
        namespace vm {
            class interpreter;

            using VMRegister = Addr;

            class vm_context {
                friend class intrpreter;

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

