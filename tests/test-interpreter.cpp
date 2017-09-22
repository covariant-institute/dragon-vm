//
// Created by kiva on 2017/9/17.
//

#include <core/runtime/thread.hpp>
#include <cassert>

#define OPCODE(X) static_cast<dvm::core::runtime::VMOpcode>(dvm::core::runtime::VMOpcodes::X)

#define T(code_name, condition_area, ...) \
    { \
        using namespace dvm::core; \
        using namespace dvm::core::runtime; \
        printf(":: Testing " code_name "\n"); \
        Byte _c[] = { __VA_ARGS__, OPCODE(ret) }; \
        thread.set_runnable(_c); \
        context.run_thread(&thread); \
        condition_area; \
        printf(":: Passed\n"); \
    }

int main() {
    using namespace dvm::core::runtime;

    VMContext context;
    Thread thread;

    T("ldc_null", {
        assert(thread.get_stack().peek_object() == context.null_object());
    }, OPCODE(ldc_null));

    T("ldc_i32", {
        assert(thread.get_stack().peek<Int32>() == (1 << 24) + (2 << 16) + (3 << 8) + 4);
    }, OPCODE(ldc_i32), 1, 2, 3, 4);

    T("ldc_i64", {
        Int32 h = (1 << 24) + (2 << 16) + (3 << 8) + 4;
        Int32 l = (5 << 24) + (6 << 16) + (7 << 8) + 8;
        assert(thread.get_stack().peek<Int64>() == (static_cast<Int64>(h) << 32) + l);
    }, OPCODE(ldc_i32), 5, 6, 7, 8, OPCODE(ldc_i32), 1, 2, 3, 4, OPCODE(ldc_i64));

    T("pop", {
        // after dropping Int32, we have a null object
        assert(thread.get_stack().peek_object() == context.null_object());
    }, OPCODE(ldc_null), OPCODE(ldc_i32), 0, 0, 0, 0, OPCODE(pop));
}
