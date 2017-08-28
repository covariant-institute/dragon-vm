#pragma once

#include <core/opcodes.hpp>

namespace dvm::core::vm {
    class opcode_provider;
    class interpreter;

    class opcode_provider {
    public:
        virtual vm_opcode get_next_opcode() = 0;
    };
}

