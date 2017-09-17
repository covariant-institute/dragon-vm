//
// Created by kiva on 2017/8/28.
//

#include "core/runtime/interpreter.hpp"

#ifdef DVM_INTERPRETATION_THREADED

#define OPCODE(X) opcode_jump_table[static_cast<Int32>(VMOpcodes::X)] = &&OPCODE_HANDLER_NAME(X);
#define OPCODE_IMPL(name) OPCODE_HANDLER_NAME(name):
#define OPCODE_NEXT goto *jump_table[*pc++];

#else

#define OPCODE(X) opcode_jump_table[static_cast<Int32>(VMOpcodes::X)] = &Interpreter::OPCODE_HANDLER_NAME(X);
#define OPCODE_IMPL(name) void Interpreter::OPCODE_HANDLER_NAME(name)()
#define OPCODE_NEXT (this->*opcode_jump_table[*pc++])();

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
            Interpreter::Interpreter() : pc(nullptr) {
                memset(opcode_jump_table, '\0', sizeof(opcode_jump_table));
            }

            void Interpreter::load_code(VMOpcode *code) {
                this->pc = code;
            }

            void Interpreter::exec() {
#ifdef DVM_INTERPRETATION_THREADED
                threaded(opcode_jump_table);
#else
#endif
            } // Interpreter::exec()

            void Interpreter::setup_opcode_handler() {
                // 处于 threaded 模式下的跳转表初始化
                // 此模式下使用 goto 做跳转
                // 由于不允许跨函数 goto
                // 所以我们必须把跳转表的初始化和解释执行放在一起
                // 用一个参数区分是初始化还是跳转

#ifdef DVM_INTERPRETATION_THREADED
                threaded(nullptr);
            } // Interpreter::setup_opcode_handler()

            void Interpreter::threaded(void **jump_table) {
                if (jump_table != nullptr) {
                    OPCODE_NEXT;
                }

                    // 用跳转标签初始化跳转表
#include <core/runtime/opcodes_def.hpp.inc>
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
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(new_instance)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(invoke_method)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ldc_null)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ldc_i32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ldc_i64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ldc_f32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ldc_f64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(pop)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ld_i32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ld_i64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ld_f32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ld_f64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ld_object)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(st_i32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(st_i64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(st_f32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(st_f64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(st_object)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(add_i32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(add_i64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(add_f32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(add_f64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(sub_i32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(sub_i64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(sub_f32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(sub_f64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(mul_i32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(mul_i64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(mul_f32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(mul_f64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(div_i32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(div_i64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(div_f32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(div_f64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(remain_i32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(remain_i64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(remain_f32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(remain_f64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(neg_i32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(neg_i64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(neg_f32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(neg_f64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(shl_i32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(shl_i64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(shr_i32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(shr_i64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ushl_i32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ushl_i64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ushr_i32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(ushr_i64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(and_i32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(and_i64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(or_i32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(or_i64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(xor_i32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(xor_i64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(i32_to_i32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(i32_to_i64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(i32_to_f32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(i32_to_f64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(i64_to_i32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(i64_to_i64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(i64_to_f32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(i64_to_f64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(f32_to_i32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(f32_to_i64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(f32_to_f32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(f32_to_f64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(f64_to_i32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(f64_to_i64)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(f64_to_f32)
                {
                    OPCODE_NEXT;
                }

                OPCODE_IMPL(f64_to_f64)
                {
                    OPCODE_NEXT;
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
