#pragma once

#include <core/opcodes.hpp>
#include <core/vm_context.hpp>

namespace dvm {
    namespace core {
        namespace vm {
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

