#include "libdvm.h"

static FILE *output = NULL;

static void emit_opcode(VMOpcode opcode) {
    VMOpcode bytes[1] = { opcode };
    fwrite(bytes, sizeof(bytes), 1, output);
}

static void emit_number(float d) {
    fwrite(&d, sizeof(d), 1, output);
}

void do_init() {
    output = fopen("d.out", "wb");

    printf("The DragonVM calc compiler.\n");
    printf("Type expressions below:\n");
    printf("\n");
}

void do_finish() {
    emit_opcode(ret);
    fclose(output);
    printf("Compile successfully\n");
}

void do_add() {
//    printf("add\n");
    emit_opcode(add_f32);
}

void do_sub() {
//    printf("sub\n");
    emit_opcode(sub_f32);
}

void do_mul() {
//    printf("mul\n");
    emit_opcode(mul_f32);
}

void do_div() {
//    printf("div\n");
    emit_opcode(div_f32);
}

void do_number(float number) {
//    printf("number %f\n", number);
    emit_opcode(ldc_f32);
    emit_number(number);
}

void do_line_break() {
    emit_opcode(sys);
}
