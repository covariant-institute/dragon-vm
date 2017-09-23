//
// Created by kiva on 2017/9/17.
//

#include <core/runtime/thread.hpp>
#include <cassert>

#define OPCODE(X) static_cast<dvm::core::runtime::VMOpcode>(dvm::core::runtime::VMOpcodes::X)

/* see opcodes_def.hpp.inc */
#define RET_CODE 0x49

#define T(code_name, condition_area, ...) \
    { \
        using namespace dvm::core; \
        using namespace dvm::core::runtime; \
        printf(":: Testing on %s\n", code_name); \
        Byte _c[] = { __VA_ARGS__, RET_CODE }; \
        thread.set_runnable(_c); \
        context.run_thread(&thread); \
        condition_area; \
        printf(":: Passed\n"); \
    }


using namespace dvm::core;
using namespace dvm::core::runtime;

UInt8 *float_bytes(Float f) {
    static UInt8 r[sizeof(f)];
    memcpy(r, &f, sizeof(r));
    return r;
}

int main() {
    Float f32 = 3.14f;
    UInt8 *fbytes = float_bytes(f32);

    VMContext context;
    Thread thread;

    Int32 h = (1 << 24) + (2 << 16) + (3 << 8) + 4;
    Int32 l = (5 << 24) + (6 << 16) + (7 << 8) + 8;
    Int64 i64 = (static_cast<Int64>(h) << 32) + l;

    T("ldc_null", {
        assert(thread.get_stack().peek_object() == context.null_object());
    },
      OPCODE(ldc_null));

    T("ldc_i32", {
        assert(thread.get_stack().peek<Int32>() == (1 << 24) + (2 << 16) + (3 << 8) + 4);
    },
      OPCODE(ldc_i32), 1, 2, 3, 4);

    T("ldc_i64", {

        assert(thread.get_stack().peek<Int64>() == i64);
    },
      OPCODE(ldc_i32), 5, 6, 7, 8,
      OPCODE(ldc_i32), 1, 2, 3, 4,
      OPCODE(ldc_i64));

    T("ldc_f32", {
        assert(thread.get_stack().peek<Float>() == f32);
    },
      OPCODE(ldc_f32), fbytes[0], fbytes[1], fbytes[2], fbytes[3]);

    T("pop", {
        // after dropping Int32, we have a null object
        assert(thread.get_stack().peek_object() == context.null_object());
    },
      OPCODE(ldc_null),
      OPCODE(ldc_i32), 0, 0, 0, 0,
      OPCODE(pop));

    T("st_i32", {
        assert(thread.get_registers().get_register(0)->get_unchecked<Int32>() == 1);
    },
      OPCODE(ldc_i32), 0, 0, 0, 1,
      OPCODE(st_i32), 0);

    T("st_i64", {
        assert(thread.get_registers().get_register(0)->get_unchecked<Int64>() == i64);
    },
      OPCODE(ldc_i32), 5, 6, 7, 8,
      OPCODE(ldc_i32), 1, 2, 3, 4,
      OPCODE(ldc_i64),
      OPCODE(st_i64), 0);

    T("st_f32", {
        assert(thread.get_registers().get_register(0)->get_unchecked<Float>() == f32);
    },
      OPCODE(ldc_f32), fbytes[0], fbytes[1], fbytes[2], fbytes[3],
      OPCODE(st_f32), 0);

    T("st_object", {
        auto obj = thread.get_registers().get_register(0)->get_unchecked<object::Object *>();
        assert(obj == context.null_object());
    },
      OPCODE(ldc_null),
      OPCODE(st_object), 0);

    T("ld_i32", {
        assert(thread.get_stack().peek<Int32>() == 1);
    },
      OPCODE(ldc_i32), 0, 0, 0, 1,
      OPCODE(st_i32), 0,
      OPCODE(pop),
      OPCODE(ld_i32), 0);

    T("ld_i64", {
        assert(thread.get_stack().peek<Int64>() == i64);
    },
      OPCODE(ldc_i32), 5, 6, 7, 8,
      OPCODE(ldc_i32), 1, 2, 3, 4,
      OPCODE(ldc_i64),
      OPCODE(st_i64), 0,
      OPCODE(pop),
      OPCODE(ld_i64), 0);

    T("ld_f32", {
        assert(thread.get_stack().peek<Float>() == f32);
    },
      OPCODE(ldc_f32), fbytes[0], fbytes[1], fbytes[2], fbytes[3],
      OPCODE(st_f32), 0,
      OPCODE(pop),
      OPCODE(ld_f32), 0);

    T("ld_object", {
        auto obj = thread.get_stack().peek_object();
        assert(obj == context.null_object());
    },
      OPCODE(ldc_null),
      OPCODE(st_object), 0,
      OPCODE(pop),
      OPCODE(ld_object), 0);

    // 1 + 2 + 3
    T("add_i32 @ 1 + 2 + 3", {
        assert(thread.get_stack().peek<Int32>() == 6);
    },
      OPCODE(ldc_i32), 0, 0, 0, 3,
      OPCODE(ldc_i32), 0, 0, 0, 2,
      OPCODE(add_i32),
      OPCODE(ldc_i32), 0, 0, 0, 1,
      OPCODE(add_i32));

    // 3 - (2 + 1)
    T("sub_i32 @ 3 - (2 + 1)", {
        assert(thread.get_stack().peek<Int32>() == 0);
    },
      OPCODE(ldc_i32), 0, 0, 0, 1,
      OPCODE(ldc_i32), 0, 0, 0, 2,
      OPCODE(add_i32),
      OPCODE(ldc_i32), 0, 0, 0, 3,
      OPCODE(sub_i32));

    // 1 - 2
    T("sub_i32 @ 1 - 2", {
        assert(thread.get_stack().peek<Int32>() == -1);
    },
      OPCODE(ldc_i32), 0, 0, 0, 2,
      OPCODE(ldc_i32), 0, 0, 0, 1,
      OPCODE(sub_i32));

    // 3 * (4 - 2)
    T("mul_i32 @ 3 * (4 - 2)", {
        assert(thread.get_stack().peek<Int32>() == 6);
    },
      OPCODE(ldc_i32), 0, 0, 0, 2,
      OPCODE(ldc_i32), 0, 0, 0, 4,
      OPCODE(sub_i32),
      OPCODE(ldc_i32), 0, 0, 0, 3,
      OPCODE(mul_i32));

    // 3 * (2 - 4)
    T("mul_i32 @ 3 * (2 - 4)", {
        assert(thread.get_stack().peek<Int32>() == -6);
    },
      OPCODE(ldc_i32), 0, 0, 0, 4,
      OPCODE(ldc_i32), 0, 0, 0, 2,
      OPCODE(sub_i32),
      OPCODE(ldc_i32), 0, 0, 0, 3,
      OPCODE(mul_i32));

    // 12 / (8 - 2)
    T("div_i32 @ 12 / (8 - 2)", {
        assert(thread.get_stack().peek<Int32>() == 2);
    },
      OPCODE(ldc_i32), 0, 0, 0, 2,
      OPCODE(ldc_i32), 0, 0, 0, 8,
      OPCODE(sub_i32),
      OPCODE(ldc_i32), 0, 0, 0, 12,
      OPCODE(div_i32));

    // 5 % 3
    T("remain_i32 @ 5 % 3", {
        assert(thread.get_stack().peek<Int32>() == 2);
    },
      OPCODE(ldc_i32), 0, 0, 0, 3,
      OPCODE(ldc_i32), 0, 0, 0, 5,
      OPCODE(remain_i32));


    Float f5 = 5.0f;
    Float f3 = 3.0f;
    Byte byte5[sizeof(Float)];
    Byte byte3[sizeof(Float)];
    memcpy(byte5, &f5, sizeof(Float));
    memcpy(byte3, &f3, sizeof(Float));

    // 5.0f % 3.0f
    T("remain_f32 @ 5.0f % 3.0f", {
        assert(thread.get_stack().peek<Float>() == 2.0f);
    },
      OPCODE(ldc_f32), 0, 0, 0, 3,
      OPCODE(ldc_f32), byte3[0], byte3[1], byte3[2], byte3[3],
      OPCODE(ldc_f32), byte5[0], byte5[1], byte5[2], byte5[3],
      OPCODE(remain_f32));

    // - (5 % 3)
    T("neg_i32 @ - (5 % 3)", {
        assert(thread.get_stack().peek<Int32>() == -2);
    },
      OPCODE(ldc_i32), 0, 0, 0, 3,
      OPCODE(ldc_i32), 0, 0, 0, 5,
      OPCODE(remain_i32),
      OPCODE(neg_i32));

    // - (5.0f % 3.0f)
    T("neg_f32 @ - (5.0f % 3.0f)", {
        assert(thread.get_stack().peek<Float>() == -2.0f);
    },
      OPCODE(ldc_f32), 0, 0, 0, 3,
      OPCODE(ldc_f32), byte3[0], byte3[1], byte3[2], byte3[3],
      OPCODE(ldc_f32), byte5[0], byte5[1], byte5[2], byte5[3],
      OPCODE(remain_f32),
      OPCODE(neg_f32));

    // 1 << 5
    T("shl_i32", {
        assert(thread.get_stack().peek<Int32>() == (1 << 5));
    },
      OPCODE(ldc_i32), 0, 0, 0, 5,
      OPCODE(ldc_i32), 0, 0, 0, 1,
      OPCODE(shl_i32));

    // 1LL << 8
    T("shl_i64", {
        assert(thread.get_stack().peek<Int64>() == (Int64(1) << 8));
    },
      OPCODE(ldc_i32), 0, 0, 0, 8,
      OPCODE(ldc_i32), 0, 0, 0, 1,
      OPCODE(ldc_i32), 0, 0, 0, 0,
      OPCODE(ldc_i64),
      OPCODE(shl_i64));

    // 4 >> 2
    T("shr_i32", {
        assert(thread.get_stack().peek<Int32>() == (4 >> 2));
    },
      OPCODE(ldc_i32), 0, 0, 0, 2,
      OPCODE(ldc_i32), 0, 0, 0, 4,
      OPCODE(shr_i32));

    // 4LL >> 2
    T("shr_i64", {
        assert(thread.get_stack().peek<Int64>() == (Int64(4) >> 2));
    },
      OPCODE(ldc_i32), 0, 0, 0, 2,
      OPCODE(ldc_i32), 0, 0, 0, 4, /* low */
      OPCODE(ldc_i32), 0, 0, 0, 0, /* high */
      OPCODE(ldc_i64),
      OPCODE(shr_i64));

    T("and_i32", {
        assert(thread.get_stack().peek<Int32>() == 0);
    },
      OPCODE(ldc_i32), 0, 0, 0, 1,
      OPCODE(ldc_i32), 0, 0, 0, 0,
      OPCODE(and_i32));

    T("and_i64", {
        assert(thread.get_stack().peek<Int64>() == Int64(0));
    },
      OPCODE(ldc_i32), 0, 0, 0, 1, /* low */
      OPCODE(ldc_i32), 0, 0, 0, 0, /* high */
      OPCODE(ldc_i64),
      OPCODE(ldc_i32), 0, 0, 0, 0, /* low */
      OPCODE(ldc_i32), 0, 0, 0, 0, /* high */
      OPCODE(ldc_i64),
      OPCODE(and_i64));

    T("or_i32", {
        assert(thread.get_stack().peek<Int32>() == 1);
    },
      OPCODE(ldc_i32), 0, 0, 0, 1,
      OPCODE(ldc_i32), 0, 0, 0, 0,
      OPCODE(or_i32));

    T("or_i64", {
        assert(thread.get_stack().peek<Int64>() == 1);
    },
      OPCODE(ldc_i32), 0, 0, 0, 1, /* low */
      OPCODE(ldc_i32), 0, 0, 0, 0, /* high */
      OPCODE(ldc_i64),
      OPCODE(ldc_i32), 0, 0, 0, 0, /* low */
      OPCODE(ldc_i32), 0, 0, 0, 0, /* high */
      OPCODE(ldc_i64),
      OPCODE(or_i64));

    T("xor_i32", {
        assert(thread.get_stack().peek<Int32>() == (1 ^ 2));
    },
      OPCODE(ldc_i32), 0, 0, 0, 2,
      OPCODE(ldc_i32), 0, 0, 0, 1,
      OPCODE(xor_i32));

    T("xor_i64", {
        assert(thread.get_stack().peek<Int64>() == (Int64(1) ^ Int64(2)));
    },
      OPCODE(ldc_i32), 0, 0, 0, 2, /* low */
      OPCODE(ldc_i32), 0, 0, 0, 0, /* high */
      OPCODE(ldc_i64),
      OPCODE(ldc_i32), 0, 0, 0, 1, /* low */
      OPCODE(ldc_i32), 0, 0, 0, 0, /* high */
      OPCODE(ldc_i64),
      OPCODE(xor_i64));
}
