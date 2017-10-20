//
// Created by kiva on 2017/9/22.
//
#pragma once

#include <core/runtime/interpreter.hpp>
#include <core/runtime/thread.hpp>
#include <core/runtime/register.hpp>
#include <core/runtime/invoke.hpp>
#include <core/runtime/throw.hpp>
#include <core/object/reference.hpp>

namespace dvm {
    namespace core {
        namespace runtime {

            class Dispatcher {
                friend class Interpreter;

            private:
                static inline void ensure_slot_in_bounds(Thread *thread, object::Object *target,
                                                         UInt8 index) {
                    object::ensure_object_valid(target);

                    if (target->is_null()) {
                        // TODO Throw npe
                        return;
                    }

                    if (index >= target->get_prototype()->member_slot_count) {
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
                    ensure_slot_in_bounds(thread, target, slot);
                    target->get_slot(slot)->set<T>(value);
                }

                template <typename T>
                static inline void set_class_slot(Thread *thread, object::Class *target, UInt8 slot, T value) {
                    ensure_index_in_bounds(thread, target, slot);
                    target->slots[slot].set<T>(value);
                }

                template <typename T>
                static inline T get_slot(Thread *thread, object::Object *target, UInt8 slot) {
                    ensure_slot_in_bounds(thread, target, slot);
                    return target->get_slot(slot)->get<T>();
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

                    auto ref = thread->stack.peek_pop<object::Reference>();
                    if (ref.is_object()) {
                        T &&value = thread->stack.peek_pop<T>();

                        set_slot<T>(thread, ref.as_object(), slot_id, value);
                    }
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

                    auto ref = thread->stack.peek_pop<object::Reference>();
                    if (ref.is_object()) {
                        T &&value = get_slot<T>(thread, ref.as_object(), slot_id);
                        thread->stack.push(std::forward<T>(value));
                    }
                }

                template <typename T>
                static inline void get_class_slot(Thread *thread) {
                    UInt16 &&class_id = thread->const_u16();
                    dvm_memory_barrier();
                    UInt8 &&slot_id = thread->const_u8();

                    T &&value = get_class_slot<T>(thread,
                                                  thread->get_context()->find_class_constant(class_id),
                                                  slot_id);
                    thread->stack.push(std::forward<T>(value));
                }

                static inline void throw_exception(Thread *thread) {
                    auto ref = thread->stack.peek_pop<object::Reference>();
                    if (ref.is_object()) {
                        Dispatcher::throw_exception(thread, ref.as_object());
                    }
                    // TODO Invalid operations
                }

                static inline void method_return_void(Thread *thread) {
                    Invocation::return_void(thread);
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
                    thread->stack.push<object::Reference>(object::Reference::make_object(object));
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
                static inline void store(Thread *thread) {
                    UInt8 &&reg = thread->const_u8();

                    // Do not use peek_pop()
                    // st_* instructions do not drop stack elements
                    thread->regs[reg]->set_unchecked(thread->stack.peek<T>());
                }

                template <typename T>
                static inline void store_at(Thread *thread) {
                    UInt16 &&offset = thread->const_u16();
                    UInt8 &&reg = thread->const_u8();

                    thread->regs[reg]->set_unchecked(*thread->stack.at<T>(offset));
                }

                template <typename T>
                static inline void load(Thread *thread) {
                    UInt8 &&reg = thread->const_u8();
                    thread->stack.push(thread->regs[reg]->get_unchecked<T>());
                }

                static inline void load_object(Thread *thread) {
                    UInt8 &&reg = thread->const_u8();
                    thread->stack.push(object::Reference::make_object(
                            thread->regs[reg]->get_unchecked<object::Object *>()
                    ));
                }

                static inline void load_array(Thread *thread) {
                    UInt8 &&reg = thread->const_u8();
                    thread->stack.push(object::Reference::make_array(
                            thread->regs[reg]->get_unchecked<object::Array *>()
                    ));
                }

                template <typename T>
                static inline void pop(Thread *thread) {
                    thread->stack.pop<T>();
                }
            };
        }
    }
}
