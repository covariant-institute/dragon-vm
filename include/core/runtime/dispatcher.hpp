//
// Created by kiva on 2017/9/22.
//
#pragma once

#include <core/runtime/slot_detector.hpp>
#include <core/runtime/interpreter.hpp>
#include <core/runtime/thread.hpp>
#include <core/runtime/register.hpp>
#include <core/runtime/invoke.hpp>
#include <core/runtime/throw.hpp>

namespace dvm {
    namespace core {
        namespace runtime {

            class Dispatcher {
                friend class Interpreter;

            private:
                static inline void ensure_index_in_bounds(Thread *thread, object::Object *target,
                                                          UInt8 index) {
                    object::ensure_object_valid(target);

                    if (target->is_null()) {
                        // TODO Throw npe
                        return;
                    }

                    if (index >= target->prototype->member_slot_count) {
                        // TODO Throw out of bounds
                        return;
                    }
                }

                static inline void ensure_index_in_bounds(Thread *thread, const object::Class *target,
                                                          UInt8 index) {
                    if (index >= target->class_slot_count) {
                        // TODO Throw out of bounds
                        return;
                    }
                }

            public:
                /* Utility functions */

                /**
                 * Jump to absolute address which is presenting an instruction.
                 *
                 * @param thread Executing thread
                 * @param new_pc The absolute address of the next instruction
                 */
                static inline void jump_to_exact(Thread *thread, Byte *new_pc) {
                    thread->pc = new_pc;
                }

                /**
                 * Jump to relative address which is presenting an instruction.
                 *
                 * @param thread Executing thread
                 * @param offset Offset to current pc, positive or negative
                 */
                static inline void jump_to_offset(Thread *thread, Int32 offset) {
                    jump_to_exact(thread, thread->pc + offset);
                }

                /**
                 * Throw an exception
                 *
                 * @param thread Executing thread
                 * @param ex Exception
                 */
                static inline void throw_exception(Thread *thread, object::Object *ex) {
                    // TODO: Abort when ex == nullptr
                    if (ex == nullptr) {
                        return;
                    }

                    Byte *handler = nullptr;
                    while (!thread->stack.empty()) {
                        handler = ThrowHelper::find_handler(thread, ex);

                        if (handler != nullptr) {
                            break;
                        }

                        // find handlers in up-level caller
                        thread->stack.remove_top_frame();
                    }


                    // handler not found
                    if (handler != nullptr) {
                        // update thread state
                        thread->exception = ex;

                        // jump to handler
                        Dispatcher::jump_to_exact(thread, handler);
                        return;
                    }

                    // TODO abort VM when handler not found.
                }

                template <typename T>
                static inline void set_slot(Thread *thread, object::Object *target, UInt8 slot, T value) {
                    ensure_index_in_bounds(thread, target, slot);
                    target->slots[slot].set<T>(value);
                }

                template <typename T>
                static inline void set_class_slot(Thread *thread, object::Class *target, UInt8 slot, T value) {
                    ensure_index_in_bounds(thread, target, slot);
                    target->slots[slot].set<T>(value);
                }

                template <typename T>
                static inline T get_slot(Thread *thread, object::Object *target, UInt8 slot) {
                    ensure_index_in_bounds(thread, target, slot);
                    return target->slots[slot].get<T>();
                }

                template <typename T>
                static inline T get_class_slot(Thread *thread, const object::Class *target, UInt8 slot) {
                    ensure_index_in_bounds(thread, target, slot);
                    return target->slots[slot].get<T>();
                }

            private:
                /* Outer interfaces to Interpreter */

                template <typename T>
                static inline void set_slot(Thread *thread) {
                    UInt8 &&slot_id = thread->const_u8();

                    object::Object *target = thread->stack.peek_object_pop();
                    T &&value = thread->stack.peek_pop<T>();

                    set_slot<T>(thread, target, slot_id, value);
                }

                template <typename T>
                static inline void set_class_slot(Thread *thread) {
                    UInt16 &&class_id = thread->const_u16();
                    dvm_memory_barrier();
                    UInt8 &&slot_id = thread->const_u8();

                    T &&value = thread->stack.peek_pop<T>();

                    auto *clazz = thread->get_context()->find_class_constant(class_id);
                    set_class_slot<T>(thread, clazz,
                                      slot_id, value);
                }

                template <typename T>
                static inline void get_slot(Thread *thread) {
                    UInt8 &&slot_id = thread->const_u8();

                    object::Object *target = thread->stack.peek_object_pop();

                    T &&value = get_slot<T>(thread, target, slot_id);
                    thread->stack.push(std::forward<T>(value));
                }

                template <typename T>
                static inline void get_class_slot(Thread *thread) {
                    UInt8 &&class_id = thread->const_u8();
                    dvm_memory_barrier();
                    UInt8 &&slot_id = thread->const_u8();

                    T &&value = get_class_slot<T>(thread,
                                                  thread->get_context()->find_class_constant(class_id),
                                                  slot_id);
                    thread->stack.push(std::forward<T>(value));
                }

                static inline void throw_exception(Thread *thread) {
                    auto *ex = thread->stack.peek_object_pop();
                    Dispatcher::throw_exception(thread, ex);
                }

                static inline void method_return_void(Thread *thread) {
                    Invocation::return_void(thread);
                }

                static inline void method_return_object(Thread *thread) {
                    object::Object *ret = thread->stack.peek_object_pop();
                    Invocation::return_from_method<object::Object *>(thread, ret);
                }

                template <typename T>
                static inline void method_return(Thread *thread) {
                    T &&ret = thread->stack.peek_pop<T>();
                    Invocation::return_from_method<T>(thread, std::forward<T>(ret));
                }

                static inline void invoke_method(Thread *thread) {
                    UInt16 name_id = thread->const_u16();
                    dvm_memory_barrier();
                    UInt16 signature_id = thread->const_u16();

                    object::Method *method =
                            Invocation::resolve_by_id(thread, name_id, signature_id);
                    Invocation::invoke_method_raw(thread, method);
                }

                static inline void new_instance(Thread *thread) {
                    UInt16 class_id = thread->const_u16();
                    auto prototype = thread->get_context()->find_class_constant(class_id);
                    auto object = prototype->new_instance();
                    thread->stack.push_object_ref(object);
                }

                template <typename Condition>
                static inline void jump(Thread *thread) {
                    Int32 &&check = thread->stack.peek_pop<Int32>();
                    Int16 offset = thread->const_i16();

                    if (Condition::get_result(check)) {
                        jump_to_offset(thread, offset);
                    }
                }

                static inline void jump0(Thread *thread) {
                    Int16 offset = thread->const_i16();
                    jump_to_offset(thread, offset);
                }

                static inline void jump0_w(Thread *thread) {
                    Int32 offset = thread->const_i32();
                    jump_to_offset(thread, offset);
                }

                template <typename FromType, typename ToType>
                static inline void cast(Thread *thread) {
                    FromType &&from = std::move(thread->stack.peek_pop<FromType>());
                    thread->stack.push<ToType>(std::forward<ToType>(from));
                }

                template <typename LhsType, typename Impl,
                        typename RhsType = LhsType, typename ResultType = LhsType>
                static inline void math(Thread *thread) {
                    LhsType &&lhs = thread->stack.peek_pop<LhsType>();
                    dvm_memory_barrier();
                    RhsType &&rhs = thread->stack.peek_pop<RhsType>();

                    ResultType &&result = Impl::get_result(std::forward<LhsType>(lhs),
                                                           std::forward<RhsType>(rhs));
                    thread->stack.push<ResultType>(std::forward<ResultType>(result));
                }

                template <typename LhsType, typename Impl, typename ResultType = LhsType>
                static inline void math1(Thread *thread) {
                    LhsType &&operand = thread->stack.peek_pop<LhsType>();

                    ResultType &&result = Impl::get_result(std::forward<LhsType>(operand));
                    thread->stack.push<ResultType>(std::forward<ResultType>(result));
                }

                template <typename T>
                static inline T load_reg(Thread *thread) {
                    UInt8 &&reg = thread->const_u8();

                    return thread->regs[reg]->get_unchecked<T>();
                }

                template <typename T>
                static inline void store(Thread *thread) {
                    UInt8 &&reg = thread->const_u8();

                    // Do not use peek_pop()
                    // st_* instructions do not drop stack elements
                    thread->regs[reg]->set_unchecked(thread->stack.peek<T>());
                }

                static inline void store_object_ref(Thread *thread) {
                    UInt8 &&reg = thread->const_u8();

                    // Do not use peek_object_pop()
                    // st_* instructions do not drop stack elements
                    thread->regs[reg]->set_unchecked(thread->stack.peek_object());
                }

                template <typename T>
                static inline void store_at(Thread *thread) {
                    UInt16 &&offset = thread->const_u16();
                    UInt8 &&reg = thread->const_u8();

                    thread->regs[reg]->set_unchecked(*thread->stack.at<T>(offset));
                }

                static inline void store_object_ref_at(Thread *thread) {
                    UInt16 &&offset = thread->const_u16();
                    UInt8 &&reg = thread->const_u8();

                    thread->regs[reg]->set_unchecked(*thread->stack.at_object(offset));
                }

                template <typename T>
                static inline void load(Thread *thread) {
                    thread->stack.push(load_reg<T>(thread));
                }

                static inline void load_object_ref(Thread *thread) {
                    thread->stack.push_object_ref(load_reg<object::Object *>(thread));
                }

                template <typename T>
                static inline void pop(Thread *thread) {
                    thread->stack.pop<T>();
                }

                static inline void pop_object(Thread *thread) {
                    thread->stack.pop_object();
                }
            };
        }
    }
}
