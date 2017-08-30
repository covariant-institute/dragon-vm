//
// Created by kiva on 2017/8/30.
//

#include <stdio.h>
#include <stdlib.h>
#include "opcodes.h"
#include "threaded_interpreter.h"

void threaded_exec(int locales, int *program) {
    int *regs = (int *) malloc(locales * sizeof(int));
    int *pc = program;
    void *opcodes_labels[OP_COUNT] = { &&op_nop, &&op_mov, &&op_add, &&op_sub, &&op_mul, &&op_div, &&op_print, &&op_exit };

    int i0, i1, i2;

#define next_op goto *opcodes_labels[*pc++];

    next_op;

    op_nop:
    next_op;

    op_mov:
    i0 = *pc++;
    i1 = *pc++;
    regs[i0] = i1;
    next_op;

    op_add:
    i0 = *pc++;
    i1 = *pc++;
    i2 = *pc++;
    regs[i0] = regs[i1] + regs[i2];
    next_op;

    op_sub:
    i0 = *pc++;
    i1 = *pc++;
    i2 = *pc++;
    regs[i0] = regs[i1] - regs[i2];
    next_op;

    op_mul:
    i0 = *pc++;
    i1 = *pc++;
    i2 = *pc++;
    regs[i0] = regs[i1] * regs[i2];
    next_op;

    op_div:
    i0 = *pc++;
    i1 = *pc++;
    i2 = *pc++;
    regs[i0] = regs[i1] / regs[i2];
    next_op;

    op_print:
    printf("%d\n", regs[*pc++]);
    next_op;

    op_exit:
    free(regs);
}
