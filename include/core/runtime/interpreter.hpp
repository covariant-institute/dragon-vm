#pragma once

#include <core/runtime/opcodes.hpp>
#include <core/runtime/vm_context.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            class opcode_provider;

            class interpreter;

            class opcode_provider {
            public:
                virtual vm_opcode get_next_opcode() = 0;
            };

            class interpreter {

            };
        }
    }
}

