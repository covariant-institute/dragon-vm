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
        thread.get_stack().new_frame(64); \
        thread.set_runnable(_c); \
        context.run_thread(&thread); \
        thread.get_stack().remove_top_frame(); \
        condition_area; \
        printf(":: Passed\n"); \
    }


using namespace dvm::core;
using namespace dvm::core::runtime;

//UInt8 *float_bytes1(Float f) {
//    static UInt8 r[sizeof(f)];
//    memcpy(r, &f, sizeof(r));
//    return r;
//}
//
//UInt8 *float_bytes2(Float f) {
//    static UInt8 r[sizeof(f)];
//    memcpy(r, &f, sizeof(r));
//    return r;
//}
//
//UInt8 *float_bytes3(Float f) {
//    static UInt8 r[sizeof(f)];
//    memcpy(r, &f, sizeof(r));
//    return r;
//}

union FloatBytes {
    Float f;
    UInt8 b[sizeof(Float)];
};

int main() {
    FloatBytes f1{ };
    f1.f = 3.14f;

    FloatBytes f2{ };
    f2.f = 4.14f;

    FloatBytes f3{ };
    f3.f = 5.14f;

//    Float float1 = 3.14f;
//    UInt8 *float_bytes1 = float_bytes1(float1);
//
//    Float float2 = 4.14f;
//    UInt8 *float_bytes2 = float_bytes2(float2);
//
//    Float float3 = 5.14f;
//    UInt8 *float_bytes3 = float_bytes3(float3);

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
        assert(thread.get_stack().peek<Float>() == f1.f);
    },
      OPCODE(ldc_f32), f1.b[0], f1.b[1], f1.b[2], f1.b[3]);

    T("pop_i32", {
        // after dropping Int32, we have a null object
        assert(thread.get_stack().peek_object() == context.null_object());
    },
      OPCODE(ldc_null),
      OPCODE(ldc_i32), 0, 0, 0, 0,
      OPCODE(pop_i32));

    T("st_i32", {
        assert(thread.get_registers()[0]->get_unchecked<Int32>() == 1);
    },
      OPCODE(ldc_i32), 0, 0, 0, 1,
      OPCODE(st_i32), 0);

    T("st_i64", {
        assert(thread.get_registers()[0]->get_unchecked<Int64>() == i64);
    },
      OPCODE(ldc_i32), 5, 6, 7, 8,
      OPCODE(ldc_i32), 1, 2, 3, 4,
      OPCODE(ldc_i64),
      OPCODE(st_i64), 0);

    T("st_f32", {
        assert(thread.get_registers()[0]->get_unchecked<Float>() == f1.f);
    },
      OPCODE(ldc_f32), f1.b[0], f1.b[1], f1.b[2], f1.b[3],
      OPCODE(st_f32), 0);

    T("st_object", {
        auto obj = thread.get_registers()[0]->get_unchecked<object::Object *>();
        assert(obj == context.null_object());
    },
      OPCODE(ldc_null),
      OPCODE(st_object), 0);

    T("ld_i32", {
        assert(thread.get_stack().peek<Int32>() == 1);
    },
      OPCODE(ldc_i32), 0, 0, 0, 1,
      OPCODE(st_i32), 0,
      OPCODE(pop_i32),
      OPCODE(ld_i32), 0);

    T("ld_i64", {
        assert(thread.get_stack().peek<Int64>() == i64);
    },
      OPCODE(ldc_i32), 5, 6, 7, 8,
      OPCODE(ldc_i32), 1, 2, 3, 4,
      OPCODE(ldc_i64),
      OPCODE(st_i64), 0,
      OPCODE(pop_i64),
      OPCODE(ld_i64), 0);

    T("ld_f32", {
        assert(thread.get_stack().peek<Float>() == f1.f);
    },
      OPCODE(ldc_f32), f1.b[0], f1.b[1], f1.b[2], f1.b[3],
      OPCODE(st_f32), 0,
      OPCODE(pop_f32),
      OPCODE(ld_f32), 0);

    T("ld_object", {
        auto obj = thread.get_stack().peek_object();
        assert(obj == context.null_object());
    },
      OPCODE(ldc_null),
      OPCODE(st_object), 0,
      OPCODE(pop_object),
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


    Float f5_0 = 5.0f;
    Float f3_0 = 3.0f;
    Byte byte5[sizeof(Float)];
    Byte byte3[sizeof(Float)];
    memcpy(byte5, &f5_0, sizeof(Float));
    memcpy(byte3, &f3_0, sizeof(Float));

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

    T("i32_to_f32", {
        assert(thread.get_stack().peek<Float>() == 3.0f);
    },
      OPCODE(ldc_i32), 0, 0, 0, 3,
      OPCODE(i32_to_f32));

    T("i32_to_f64", {
        assert(thread.get_stack().peek<Double>() == 99.0);
    },
      OPCODE(ldc_i32), 0, 0, 0, 99,
      OPCODE(i32_to_f64));

    T("i32_to_i64", {
        assert(thread.get_stack().peek<Int64>() == Int64(3));
    },
      OPCODE(ldc_i32), 0, 0, 0, 3,
      OPCODE(i32_to_i64));

    T("stp_i32", {
        assert(thread.get_registers()[4]->get_unchecked<Int32>() == 2);
    },
      OPCODE(ldc_i32), 0, 0, 0, 1, /* bp - 1 * sizeof(Int32) */
      OPCODE(ldc_i32), 0, 0, 0, 2, /* bp - 2 * sizeof(Int32) */
      OPCODE(ldc_i32), 0, 0, 0, 3, /* bp - 3 * sizeof(Int32) */
      OPCODE(stp_i32), 0, sizeof(Int32) * 2, 4);

    T("cmp_i32 @ value1 == value2", {
        assert(thread.get_stack().peek<Int32>() == 0);
    },
      OPCODE(ldc_i32), 0, 0, 0, 77, /* value2 */
      OPCODE(ldc_i32), 0, 0, 0, 77, /* value1 */
      OPCODE(cmp_i32));

    T("cmp_i32 @ value1 > value2", {
        assert(thread.get_stack().peek<Int32>() == 1);
    },
      OPCODE(ldc_i32), 0, 0, 0, 77, /* value2 */
      OPCODE(ldc_i32), 0, 0, 0, 80, /* value1 */
      OPCODE(cmp_i32));

    T("cmp_i32 @ value1 < value2", {
        assert(thread.get_stack().peek<Int32>() == -1);
    },
      OPCODE(ldc_i32), 0, 0, 0, 77, /* value2 */
      OPCODE(ldc_i32), 0, 0, 0, 70, /* value1 */
      OPCODE(cmp_i32));

    T("cmp_lt_f32 @ value1 < value2", {
        assert(thread.get_stack().peek<Int32>() == 0);
    },
      OPCODE(ldc_f32), f3.b[1], f3.b[2], f3.b[3], f3.b[4], /* value2 */
      OPCODE(ldc_f32), f2.b[1], f2.b[2], f2.b[3], f2.b[4], /* value1 */
      OPCODE(cmp_lt_f32));

    T("cmp_lt_f32 @ value1 < value2", {
        assert(thread.get_stack().peek<Int32>() == 1);
    },
      OPCODE(ldc_f32), f1.b[1], f1.b[2], f1.b[3], f1.b[4], /* value2 */
      OPCODE(ldc_f32), f2.b[1], f2.b[2], f2.b[3], f2.b[4], /* value1 */
      OPCODE(cmp_lt_f32));

    T("cmp_gt_f32 @ value1 > value2", {
        assert(thread.get_stack().peek<Int32>() == 1);
    },
      OPCODE(ldc_f32), f3.b[1], f3.b[2], f3.b[3], f3.b[4], /* value2 */
      OPCODE(ldc_f32), f2.b[1], f2.b[2], f2.b[3], f2.b[4], /* value1 */
      OPCODE(cmp_gt_f32));

    T("cmp_gt_f32 @ value1 > value2", {
        assert(thread.get_stack().peek<Int32>() == 0);
    },
      OPCODE(ldc_f32), f1.b[1], f1.b[2], f1.b[3], f1.b[4], /* value2 */
      OPCODE(ldc_f32), f2.b[1], f2.b[2], f2.b[3], f2.b[4], /* value1 */
      OPCODE(cmp_gt_f32));
}
