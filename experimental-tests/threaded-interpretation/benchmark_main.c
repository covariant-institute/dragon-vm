//
// Created by kiva on 2017/8/30.
//

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/time.h>

#include "threaded_interpreter.h"
#include "decode-dispatch-interpreter.h"

#include "opcodes.h"

long diff_time_ms(struct timeval *start, struct timeval *end) {
    return (1000000 * (end->tv_sec - start->tv_sec) + end->tv_usec - start->tv_usec) / 1000;
}

int main() {
    // 10亿条指令
    int reg_count = 16;

    printf("* Generating random code...   ");
    int *program = (int *) malloc(sizeof(int) * 1000000010);
    bzero(program, sizeof(int) * 1000000010);
    for (int reg = 0; reg < reg_count; ++reg) {
        int i = reg * 3;
        program[i + 0] = OP_MOV;
        program[i + 1] = reg;
        program[i + 2] = reg;
    }
    for (int i = reg_count * 3; i < 1000000000;) {
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
    program[1000000009] = OP_EXIT;
    printf("Done.\n");

    printf("* Preheating system...   ");
    threaded_exec(reg_count, program);
    decode_dispatch_exec(reg_count, program);
    printf("Dont.\n");

    printf("* Start benchmarking...\n");
    struct timeval start, end;
    long time_diff = 0;
    const int bench_times = 10;
    long score_threaded[bench_times] = { 0 };
    long score_dispatch[bench_times] = { 0 };

    for (int i = 0; i < bench_times; i++) {
        gettimeofday(&start, NULL);
        threaded_exec(reg_count, program);
        gettimeofday(&end, NULL);
        time_diff = diff_time_ms(&start, &end);
        printf("    Benchmark #%d: threaded-exec: %ld\n", i, time_diff);
        score_threaded[i] = time_diff;

        gettimeofday(&start, NULL);
        decode_dispatch_exec(reg_count, program);
        gettimeofday(&end, NULL);
        time_diff = diff_time_ms(&start, &end);
        printf("    Benchmark #%d: dispatch-exec: %ld\n", i, diff_time_ms(&start, &end));
        score_dispatch[i] = time_diff;
    }

    long sum_threaded = 0;
    long sum_dispatch = 0;
    for (int i = 0; i < bench_times; i++) {
        sum_threaded += score_threaded[i];
        sum_dispatch += score_dispatch[i];
    }

    printf("* Benchmark results:\n");
    printf("    Threaded average: %.2lf\n", sum_threaded / bench_times / 1.0);
    printf("    Dispatch average: %.2lf\n", sum_dispatch / bench_times / 1.0);
}
