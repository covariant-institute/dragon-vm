//
// Created by kiva on 2017/10/5.
//

#undef DVM_LITTLE_ENDIAN

#include <core/dcx/byte_ordered_reader.hpp>
#include <cstdio>

using dvm::core::Byte;
using dvm::core::dcx::ByteOrderedReader;

void show(Byte *x, int length) {
    for (int i = 0; i < length; ++i) {
        printf("%d ", x[i]);
    }
    printf("\n");
}

int main() {
    Byte x[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    show(x, sizeof(x));

    ByteOrderedReader::to_little_endian(x, sizeof(x));
    show(x, sizeof(x));

    Byte x1[] = { 1, 2, 3, 4, 5 };
    show(x1, sizeof(x1));

    ByteOrderedReader::to_little_endian(x1, sizeof(x1));
    show(x1, sizeof(x1));
}
