//
// Created by kiva on 2017/9/16.
//

#include <core/runtime/opcodes.hpp>

int main() {
    using namespace dvm::core::runtime;

    printf("| Mnemonic | Opcode (in hex) | Opcode (in binary) | Other bytes ([count]: [operand labels]) | Stack ([before] -> [after]) | Description |\n");
    printf("|:--------:|:---------------:|:------------------:|:---------------------------------------:|:---------------------------:|:-----------:|\n");

    for (int i = 0; i < VM_OPCODES_NUMBER; ++i) {
        printf("%s|%x|%x|other bytes|stack|desc\n", VM_OPCODE_NAMES[i], i, i);
    }
}