//
// Created by kiva on 2017/8/30.
//

#include <stdio.h>
#include <stdlib.h>
#include "opcodes.h"
#include "decode-dispatch-interpreter.h"

void decode_dispatch_exec(int locales, int *program) {
    int *regs = (int *) malloc(locales * sizeof(int));
    int *pc = program;

    int i0, i1, i2;

    for (;;) {
        switch (*pc++) {
            case OP_MOV:
                i0 = *pc++;
                i1 = *pc++;
                regs[i0] = i1;
                break;

            case OP_ADD:
                i0 = *pc++;
                i1 = *pc++;
                i2 = *pc++;
                regs[i0] = regs[i1] + regs[i2];
                break;

            case OP_SUB:
                i0 = *pc++;
                i1 = *pc++;
                i2 = *pc++;
                regs[i0] = regs[i1] - regs[i2];
                break;

            case OP_MUL:
                i0 = *pc++;
                i1 = *pc++;
                i2 = *pc++;
                regs[i0] = regs[i1] * regs[i2];
                break;

            case OP_DIV:
                i0 = *pc++;
                i1 = *pc++;
                i2 = *pc++;
                regs[i0] = regs[i1] / regs[i2];
                break;

            case OP_PRINT:
                printf("%d\n", regs[*pc++]);
                break;

            case OP_NOP:
                break;

            case OP_EXIT:
                goto end;

            default:
                break;
        }
    }

    end:
    free(regs);
}
