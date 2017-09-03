//
// Created by kiva on 2017/8/30.
//

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/time.h>

#include "threaded-interpreter.h"
#include "decode-dispatch-interpreter.h"

#include "opcodes.h"

long diff_time_ms(struct timeval *start, struct timeval *end) {
    return (1000000 * (end->tv_sec - start->tv_sec) + end->tv_usec - start->tv_usec) / 1000;
}

int main(int argc, const char **argv) {
    int reg_count = 16;

    // 10亿条指令
    int code_count = 1000000000;
    const int code_edge_expand = 100;

    if (argc > 1) {
        code_count = atoi(argv[1]);
    }

    printf("* Using code count: %d\n", code_count);
    printf("* Generating random code...   ");
    fflush(stdout);

    int *program = (int *) malloc(sizeof(int) * (code_count + code_edge_expand));
    bzero(program, sizeof(int) * (code_count + code_edge_expand));

    for (int reg = 0; reg < reg_count; ++reg) {
        int i = reg * 3;
        program[i + 0] = OP_MOV;
        program[i + 1] = reg;
        program[i + 2] = reg;
    }

    for (int i = reg_count * 3; i < code_count;) {
        int random_opcode = i % OP_DIV;
        int random_reg = i % reg_count;
        if (random_opcode < OP_ADD) {
            random_opcode += OP_ADD;
        }
        program[i++] = random_opcode;
        program[i++] = random_reg;
        program[i++] = random_reg;
        program[i++] = random_reg;
    }
    program[code_count + code_edge_expand - 1] = OP_EXIT;
    printf("Done.\n");

    printf("* Preheating system...   ");
    fflush(stdout);
    threaded_exec(reg_count, program);
    decode_dispatch_exec(reg_count, program);
    printf("Done.\n");

    printf("* Start benchmarking...\n");
    struct timeval start, end;
    long time_diff = 0;
    const int bench_times = 10;
    long sum_threaded = 0;
    long sum_dispatch = 0;

    for (int i = 0; i < bench_times; i++) {
        gettimeofday(&start, NULL);
        threaded_exec(reg_count, program);
        gettimeofday(&end, NULL);
        time_diff = diff_time_ms(&start, &end);
        printf("    Benchmark #%d: threaded-exec: %ld\n", i, time_diff);
        sum_threaded += time_diff;

        gettimeofday(&start, NULL);
        decode_dispatch_exec(reg_count, program);
        gettimeofday(&end, NULL);
        time_diff = diff_time_ms(&start, &end);
        printf("    Benchmark #%d: dispatch-exec: %ld\n", i, diff_time_ms(&start, &end));
        sum_dispatch += time_diff;
    }

    printf("* Benchmark results:\n");
    printf("    Threaded average: %.2lf\n", sum_threaded / bench_times / 1.0);
    printf("    Dispatch average: %.2lf\n", sum_dispatch / bench_times / 1.0);
}
