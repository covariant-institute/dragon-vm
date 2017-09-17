//
// Created by kiva on 2017/9/17.
//

#include <core/runtime/opcodes.hpp>
using namespace dvm::core::runtime;

int main() {
    for (int i = 0; i < VM_OPCODES_NUMBER; ++i) {
        printf("OPCODE_IMPL(%s)\n{\n    OPCODE_NEXT;\n}\n\n", VM_OPCODE_NAMES[i]);
    }
}

