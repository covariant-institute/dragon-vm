//
// Created by kiva on 2017/8/28.
//

#include "core/runtime/interpreter.hpp"

#ifdef DVM_INTERPRETATION_THREADED

#define OPCODE(X) opcode_jump_table[static_cast<Int32>(VMOpcodes::X)] = &&OPCODE_HANDLER_NAME(X);
#define OPCODE_IMPL(name) OPCODE_HANDLER_NAME(name):
#define OPCODE_NEXT goto *opcode_jump_table[*thread->pc++];
#define OPCODE_RETURN return;

#else

#define OPCODE(X) opcode_jump_table[static_cast<Int32>(VMOpcodes::X)] = &Interpreter::OPCODE_HANDLER_NAME(X);
#define OPCODE_IMPL(name) void Interpreter::OPCODE_HANDLER_NAME(name)()
#define OPCODE_NEXT (this->*opcode_jump_table[*thread->pc++])();
#define OPCODE_RETURN return;

#endif

#define INTS_OPCODES_PREFIX(PREFIX) \
            OPCODE(PREFIX##_i32) \
            OPCODE(PREFIX##_i64)

#define FLOATS_OPCODES_PREFIX(PREFIX) \
            OPCODE(PREFIX##_f32) \
            OPCODE(PREFIX##_f64)

#define TYPED_OPCODES_PREFIX(PREFIX) \
            INTS_OPCODES_PREFIX(PREFIX) \
            FLOATS_OPCODES_PREFIX(PREFIX)

namespace dvm {
    namespace core {
        namespace runtime {
            Interpreter::Interpreter() {
                memset(opcode_jump_table, '\0', sizeof(opcode_jump_table));
                setup_opcode_handler();
            }

            void Interpreter::exec(Thread *thread) {
                if (thread == nullptr || thread->pc == nullptr) {
                    throw dvm::core::exception(DVM_RUNTIME_INVALID_CODE);
                }

#ifdef DVM_INTERPRETATION_THREADED
                threaded(thread, opcode_jump_table);
#else
                OPCODE_NEXT;
#endif
            } // Interpreter::exec()

            void Interpreter::setup_opcode_handler() {
                // 处于 threaded 模式下的跳转表初始化
                // 此模式下使用 goto 做跳转
                // 由于不允许跨函数 goto
                // 所以我们必须把跳转表的初始化和解释执行放在一起
                // 用一个参数区分是初始化还是跳转

#ifdef DVM_INTERPRETATION_THREADED
                threaded(nullptr, nullptr);
            } // Interpreter::setup_opcode_handler()

            void Interpreter::threaded(Thread *thread, void **jump_table) {
                if (jump_table != nullptr) {
                    OPCODE_NEXT;
                }

                    // 用跳转标签初始化跳转表
#include <core/runtime/opcodes_def.hpp.inc>
                return;
#else

                // 非 threaded 模式下的跳转表初始化
                // 非 threaded 模式下采用 函数指针 做跳转
#include <core/runtime/opcodes_def.hpp.inc>

#endif /* DVM_INTERPRETATION_THREADED */

#ifndef DVM_INTERPRETATION_THREADED
                } // Interpreter::setup_opcode_handler()
#endif

                /**
                 * TODO: 在这里实现每个指令
                 */
                OPCODE_IMPL(nop)
                {
                    printf("[pc = %p]: nop\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(new_instance)
                {
                    printf("[pc = %p]: new_instance\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(invoke_method)
                {
                    printf("[pc = %p]: invoke_method\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ldc_null)
                {
                    printf("[pc = %p]: ldc_null\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ldc_i32)
                {
                    printf("[pc = %p]: ldc_i32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ldc_i64)
                {
                    printf("[pc = %p]: ldc_i64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ldc_f32)
                {
                    printf("[pc = %p]: ldc_f32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ldc_f64)
                {
                    printf("[pc = %p]: ldc_f64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(pop)
                {
                    printf("[pc = %p]: pop\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ld_i32)
                {
                    printf("[pc = %p]: ld_i32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ld_i64)
                {
                    printf("[pc = %p]: ld_i64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ld_f32)
                {
                    printf("[pc = %p]: ld_f32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ld_f64)
                {
                    printf("[pc = %p]: ld_f64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ld_object)
                {
                    printf("[pc = %p]: ld_object\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(st_i32)
                {
                    printf("[pc = %p]: st_i32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(st_i64)
                {
                    printf("[pc = %p]: st_i64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(st_f32)
                {
                    printf("[pc = %p]: st_f32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(st_f64)
                {
                    printf("[pc = %p]: st_f64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(st_object)
                {
                    printf("[pc = %p]: st_object\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(add_i32)
                {
                    printf("[pc = %p]: add_i32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(add_i64)
                {
                    printf("[pc = %p]: add_i64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(add_f32)
                {
                    printf("[pc = %p]: add_f32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(add_f64)
                {
                    printf("[pc = %p]: add_f64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(sub_i32)
                {
                    printf("[pc = %p]: sub_i32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(sub_i64)
                {
                    printf("[pc = %p]: sub_i64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(sub_f32)
                {
                    printf("[pc = %p]: sub_f32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(sub_f64)
                {
                    printf("[pc = %p]: sub_f64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(mul_i32)
                {
                    printf("[pc = %p]: mul_i32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(mul_i64)
                {
                    printf("[pc = %p]: mul_i64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(mul_f32)
                {
                    printf("[pc = %p]: mul_f32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(mul_f64)
                {
                    printf("[pc = %p]: mul_f64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(div_i32)
                {
                    printf("[pc = %p]: div_i32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(div_i64)
                {
                    printf("[pc = %p]: div_i64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(div_f32)
                {
                    printf("[pc = %p]: div_f32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(div_f64)
                {
                    printf("[pc = %p]: div_f64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(remain_i32)
                {
                    printf("[pc = %p]: remain_i32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(remain_i64)
                {
                    printf("[pc = %p]: remain_i64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(remain_f32)
                {
                    printf("[pc = %p]: remain_f32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(remain_f64)
                {
                    printf("[pc = %p]: remain_f64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(neg_i32)
                {
                    printf("[pc = %p]: neg_i32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(neg_i64)
                {
                    printf("[pc = %p]: neg_i64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(neg_f32)
                {
                    printf("[pc = %p]: neg_f32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(neg_f64)
                {
                    printf("[pc = %p]: neg_f64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(shl_i32)
                {
                    printf("[pc = %p]: shl_i32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(shl_i64)
                {
                    printf("[pc = %p]: shl_i64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(shr_i32)
                {
                    printf("[pc = %p]: shr_i32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(shr_i64)
                {
                    printf("[pc = %p]: shr_i64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ushl_i32)
                {
                    printf("[pc = %p]: ushl_i32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ushl_i64)
                {
                    printf("[pc = %p]: ushl_i64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ushr_i32)
                {
                    printf("[pc = %p]: ushr_i32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ushr_i64)
                {
                    printf("[pc = %p]: ushr_i64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(and_i32)
                {
                    printf("[pc = %p]: and_i32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(and_i64)
                {
                    printf("[pc = %p]: and_i64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(or_i32)
                {
                    printf("[pc = %p]: or_i32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(or_i64)
                {
                    printf("[pc = %p]: or_i64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(xor_i32)
                {
                    printf("[pc = %p]: xor_i32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(xor_i64)
                {
                    printf("[pc = %p]: xor_i64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(i32_to_i32)
                {
                    printf("[pc = %p]: i32_to_i32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(i32_to_i64)
                {
                    printf("[pc = %p]: i32_to_i64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(i32_to_f32)
                {
                    printf("[pc = %p]: i32_to_f32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(i32_to_f64)
                {
                    printf("[pc = %p]: i32_to_f64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(i64_to_i32)
                {
                    printf("[pc = %p]: i64_to_i32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(i64_to_i64)
                {
                    printf("[pc = %p]: i64_to_i64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(i64_to_f32)
                {
                    printf("[pc = %p]: i64_to_f32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(i64_to_f64)
                {
                    printf("[pc = %p]: i64_to_f64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(f32_to_i32)
                {
                    printf("[pc = %p]: f32_to_i32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(f32_to_i64)
                {
                    printf("[pc = %p]: f32_to_i64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(f32_to_f32)
                {
                    printf("[pc = %p]: f32_to_f32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(f32_to_f64)
                {
                    printf("[pc = %p]: f32_to_f64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(f64_to_i32)
                {
                    printf("[pc = %p]: f64_to_i32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(f64_to_i64)
                {
                    printf("[pc = %p]: f64_to_i64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(f64_to_f32)
                {
                    printf("[pc = %p]: f64_to_f32\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(f64_to_f64)
                {
                    printf("[pc = %p]: f64_to_f64\n", thread->pc);
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ret)
                {
                    printf("[pc = %p]: ret\n", thread->pc);
                    OPCODE_RETURN;
                }

                OPCODE_IMPL(ret_object)
                {
                    printf("[pc = %p]: ret_object\n", thread->pc);
                    OPCODE_RETURN;
                }

                OPCODE_IMPL(ret_i32)
                {
                    printf("[pc = %p]: ret_i32\n", thread->pc);
                    OPCODE_RETURN;
                }

                OPCODE_IMPL(ret_i64)
                {
                    printf("[pc = %p]: ret_i64\n", thread->pc);
                    OPCODE_RETURN;
                }

                OPCODE_IMPL(ret_f32)
                {
                    printf("[pc = %p]: ret_f32\n", thread->pc);
                    OPCODE_RETURN;
                }

                OPCODE_IMPL(ret_f64)
                {
                    printf("[pc = %p]: ret_f64\n", thread->pc);
                    OPCODE_RETURN;
                }


#ifdef DVM_INTERPRETATION_THREADED
            } // Interpreter::threaded()
#endif
        } // runtime
    } // core
} // dvm

#undef INTS_OPCODES_PREFIX
#undef FLOATS_OPCODES_PREFIX
#undef TYPED_OPCODES_PREFIX
#undef OPCODE
