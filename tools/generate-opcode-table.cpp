//
// Created by kiva on 2017/9/16.
//

#include <core/runtime/opcodes.hpp>
using namespace dvm::core::runtime;

void binary(unsigned char n) {
    for (int shift = sizeof(unsigned char) * 8 - 1; shift >= 0; shift--) {
        if ((n >> shift) & 1) {
            printf("1");
        } else {
            printf("0");
        }
        if (shift == 4) {
            printf(" ");
        }
    }
}

int main() {
    printf("| Mnemonic | Opcode (in hex) | Opcode (in binary) | Other bytes ([count]: [operand labels]) | Stack ([before] -> [after]) | Description |\n");
    printf("|:--------:|:---------------:|:------------------:|:---------------------------------------:|:---------------------------:|:-----------:|\n");

    char s[9];

    for (int i = 0; i < VM_OPCODES_NUMBER; ++i) {
        printf("| %-22s |   %02x   |   ", VM_OPCODE_NAMES[i], i);
        binary((unsigned char) i);
        printf("   |        | → | desc |\n");
    }
}