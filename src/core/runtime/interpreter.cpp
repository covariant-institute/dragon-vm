//
// Created by kiva on 2017/8/28.
//

#include <core/runtime/interpreter.hpp>
#include <core/runtime/thread.hpp>
#include <core/runtime/vm_context.hpp>
#include <core/runtime/utils.hpp>
#include <core/runtime/math.hpp>
#include <core/runtime/jump.hpp>

#ifdef DVM_INTERPRETATION_THREADED

#define OPCODE(X) opcode_jump_table[static_cast<Int32>(VMOpcodes::X)] = &&OPCODE_HANDLER_NAME(X);
#define OPCODE_IMPL(name) OPCODE_HANDLER_NAME(name):
#define OPCODE_NEXT(context) goto *opcode_jump_table[*thread->pc++];
#define OPCODE_RETURN() return;

#else

#define OPCODE(X) opcode_jump_table[static_cast<Int32>(VMOpcodes::X)] = &Interpreter::OPCODE_HANDLER_NAME(X);
#define OPCODE_IMPL(name) void Interpreter::OPCODE_HANDLER_NAME(name)(Thread *thread, VMContext *context)
#define OPCODE_NEXT(context) (this->*opcode_jump_table[*thread->pc++])(thread, context);
#define OPCODE_RETURN() return;

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
                    throw dvm::core::Exception(DVM_RUNTIME_INVALID_CODE);
                }

                VMContext *context = thread->get_context();
                if (context == nullptr) {
                    throw dvm::core::Exception(DVM_RUNTIME_INVALID_ENV);
                }

#ifdef DVM_INTERPRETATION_THREADED
                threaded(thread, context, opcode_jump_table);
#else
                OPCODE_NEXT(context);
#endif
            } // Interpreter::exec()

            void Interpreter::setup_opcode_handler() {
                // 处于 threaded 模式下的跳转表初始化
                // 此模式下使用 goto 做跳转
                // 由于不允许跨函数 goto
                // 所以我们必须把跳转表的初始化和解释执行放在一起
                // 用一个参数区分是初始化还是跳转

#ifdef DVM_INTERPRETATION_THREADED
                threaded(nullptr, nullptr, nullptr);
            } // Interpreter::setup_opcode_handler()

            void Interpreter::threaded(Thread *thread, VMContext *context, void **jump_table) {
                if (jump_table != nullptr && context != nullptr && thread != nullptr) {
                    OPCODE_NEXT(context);
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
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(new_instance)
                {
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(invoke_method)
                {
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(ldc_null)
                {
                    thread->stack.push_object_ref(context->null_object());
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(ldc_i32)
                {
                    thread->stack.push(thread->const_i32());
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(ldc_i64)
                {
                    auto h = thread->stack.peek_pop<Int32>();
                    auto l = thread->stack.peek_pop<Int32>();
                    thread->stack.push<Int64>((static_cast<Int64>(h) << 32) + l);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(ldc_f32)
                {
                    thread->stack.push(thread->const_f32());
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(ldc_f64)
                {
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(sys)
                {
                    // TODO: System Call
                    // Current is to print stack top element as Float
                    printf("%lf\n", thread->stack.peek_pop<Float>());
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(ld_i32)
                {
                    Utils::load<Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(ld_i64)
                {
                    Utils::load<Int64>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(ld_f32)
                {
                    Utils::load<Float>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(ld_f64)
                {
                    Utils::load<Double>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(ld_object)
                {
                    Utils::load_object_ref(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(st_i32)
                {
                    Utils::store<Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(st_i64)
                {
                    Utils::store<Int64>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(st_f32)
                {
                    Utils::store<Float>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(st_f64)
                {
                    Utils::store<Double>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(st_object)
                {
                    Utils::store_object_ref(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(add_i32)
                {
                    Utils::math<Int32, MathAdd<Int32> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(add_i64)
                {
                    Utils::math<Int64, MathAdd<Int64> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(add_f32)
                {
                    Utils::math<Float, MathAdd<Float> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(add_f64)
                {
                    Utils::math<Double, MathAdd<Double> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(sub_i32)
                {
                    Utils::math<Int32, MathSub<Int32> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(sub_i64)
                {
                    Utils::math<Int64, MathSub<Int64> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(sub_f32)
                {
                    Utils::math<Float, MathSub<Float> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(sub_f64)
                {
                    Utils::math<Double, MathSub<Double> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(mul_i32)
                {
                    Utils::math<Int32, MathMul<Int32> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(mul_i64)
                {
                    Utils::math<Int64, MathMul<Int64> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(mul_f32)
                {
                    Utils::math<Float, MathMul<Float> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(mul_f64)
                {
                    Utils::math<Double, MathMul<Double> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(div_i32)
                {
                    Utils::math<Int32, MathDiv<Int32> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(div_i64)
                {
                    Utils::math<Int64, MathDiv<Int64> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(div_f32)
                {
                    Utils::math<Float, MathDiv<Float> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(div_f64)
                {
                    Utils::math<Double, MathDiv<Double> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(remain_i32)
                {
                    Utils::math<Int32, MathRemain<Int32> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(remain_i64)
                {
                    Utils::math<Int64, MathRemain<Int64> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(remain_f32)
                {
                    Utils::math<Float, MathRemain<Float> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(remain_f64)
                {
                    Utils::math<Double, MathRemain<Double> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(neg_i32)
                {
                    Utils::math1<Int32, MathNegate<Int32> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(neg_i64)
                {
                    Utils::math1<Int64, MathNegate<Int64> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(neg_f32)
                {
                    Utils::math1<Float, MathNegate<Float> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(neg_f64)
                {
                    Utils::math1<Double, MathNegate<Double> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(shl_i32)
                {
                    Utils::math<Int32, MathShiftLeft<Int32>, Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(shl_i64)
                {
                    Utils::math<Int64, MathShiftLeft<Int64>, Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(shr_i32)
                {
                    Utils::math<Int32, MathShiftRight<Int32>, Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(shr_i64)
                {
                    Utils::math<Int64, MathShiftRight<Int64>, Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(ushl_i32)
                {
                    Utils::math<Int32, MathShiftLeftUnsigned<Int32>, Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(ushl_i64)
                {
                    Utils::math<Int64, MathShiftLeftUnsigned<Int64>, Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(ushr_i32)
                {
                    Utils::math<Int32, MathShiftRightUnsigned<Int32>, Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(ushr_i64)
                {
                    Utils::math<Int64, MathShiftRightUnsigned<Int64>, Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(and_i32)
                {
                    Utils::math<Int32, MathAnd<Int32> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(and_i64)
                {
                    Utils::math<Int64, MathAnd<Int64> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(or_i32)
                {
                    Utils::math<Int32, MathOr<Int32> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(or_i64)
                {
                    Utils::math<Int64, MathOr<Int64> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(xor_i32)
                {
                    Utils::math<Int32, MathXor<Int32> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(xor_i64)
                {
                    Utils::math<Int64, MathXor<Int64> >(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(i32_to_i32)
                {
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(i32_to_i64)
                {
                    Utils::cast<Int32, Int64>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(i32_to_f32)
                {
                    Utils::cast<Int32, Float>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(i32_to_f64)
                {
                    Utils::cast<Int32, Double>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(i64_to_i32)
                {
                    Utils::cast<Int64, Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(i64_to_i64)
                {
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(i64_to_f32)
                {
                    Utils::cast<Int64, Float>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(i64_to_f64)
                {
                    Utils::cast<Int64, Double>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(f32_to_i32)
                {
                    Utils::cast<Float, Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(f32_to_i64)
                {
                    Utils::cast<Float, Int64>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(f32_to_f32)
                {
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(f32_to_f64)
                {
                    Utils::cast<Float, Double>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(f64_to_i32)
                {
                    Utils::cast<Double, Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(f64_to_i64)
                {
                    Utils::cast<Double, Int64>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(f64_to_f32)
                {
                    Utils::cast<Double, Float>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(f64_to_f64)
                {
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(ret)
                {
                    OPCODE_RETURN();
                }

                OPCODE_IMPL(ret_object)
                {
                    OPCODE_RETURN();
                }

                OPCODE_IMPL(ret_i32)
                {
                    OPCODE_RETURN();
                }

                OPCODE_IMPL(ret_i64)
                {
                    OPCODE_RETURN();
                }

                OPCODE_IMPL(ret_f32)
                {
                    OPCODE_RETURN();
                }

                OPCODE_IMPL(ret_f64)
                {
                    OPCODE_RETURN();
                }

                OPCODE_IMPL(jump)
                {
                    Utils::jump0(thread, false);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(jump_ret)
                {
                    Utils::jump0(thread, true);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(jump_eq)
                {
                    Utils::jump<JumpConditionEq>(thread, false);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(jump_ne)
                {
                    Utils::jump<JumpConditionNe>(thread, false);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(jump_gt)
                {
                    Utils::jump<JumpConditionGt>(thread, false);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(jump_ge)
                {
                    Utils::jump<JumpConditionGe>(thread, false);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(jump_lt)
                {
                    Utils::jump<JumpConditionLt>(thread, false);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(jump_le)
                {
                    Utils::jump<JumpConditionLe>(thread, false);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(cmp_i32)
                {
                    Utils::math<Int32, MathCompare<Int32>, Int32, Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(cmp_i64)
                {
                    Utils::math<Int64, MathCompare<Int64>, Int64, Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(cmp_lt_f32)
                {
                    Utils::math<Float, MathCompareLessThan<Float>, Float, Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(cmp_lt_f64)
                {
                    Utils::math<Double, MathCompareLessThan<Double>, Double, Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(cmp_gt_f32)
                {
                    Utils::math<Float, MathCompareGreaterThan<Float>, Float, Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(cmp_gt_f64)
                {
                    Utils::math<Double, MathCompareGreaterThan<Double>, Double, Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(cmp_object)
                {
                    Utils::math<object::Object *,
                            MathCompare<object::Object *, object::Object *>,
                            object::Object *, Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(cmp_nn_object)
                {
                    Utils::math1<object::Object *,
                            MathCompareNotNull, Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(stp_i32)
                {
                    Utils::store_at<Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(stp_i64)
                {
                    Utils::store_at<Int64>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(stp_f32)
                {
                    Utils::store_at<Float>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(stp_f64)
                {
                    Utils::store_at<Double>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(stp_object)
                {
                    Utils::store_object_ref_at(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(pop_i32)
                {
                    Utils::pop<Int32>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(pop_i64)
                {
                    Utils::pop<Int64>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(pop_f32)
                {
                    Utils::pop<Float>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(pop_f64)
                {
                    Utils::pop<Double>(thread);
                    OPCODE_NEXT(context);
                }

                OPCODE_IMPL(pop_object)
                {
                    Utils::pop_object(thread);
                    OPCODE_NEXT(context);
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
