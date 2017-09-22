//
// Created by kiva on 2017/9/17.
//

#include <core/runtime/thread.hpp>
#include <cassert>

#define OPCODE(X) static_cast<dvm::core::runtime::VMOpcode>(dvm::core::runtime::VMOpcodes::X)

#define RET_CODE 0x49

#define T(code_name, condition_area, ...) \
    { \
        using namespace dvm::core; \
        using namespace dvm::core::runtime; \
        printf(":: Testing on " code_name "\n"); \
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
}
