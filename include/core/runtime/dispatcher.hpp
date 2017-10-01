//
// Created by kiva on 2017/9/22.
//
#pragma once

#include <core/runtime/interpreter.hpp>
#include <core/runtime/thread.hpp>
#include <core/runtime/register.hpp>
#include <core/runtime/invoke.hpp>

namespace dvm {
    namespace core {
        namespace runtime {

            class Dispatcher {
                friend class Interpreter;

            public:
                /* Utility functions */

                static inline void jump_to_exact(Thread *thread, Byte *new_pc) {
                    thread->pc = new_pc;
                }

                static inline void jump_to_offset(Thread *thread, Int32 offset) {
                    jump_to_exact(thread, thread->pc + offset);
                }

                static inline void invoke_method(Thread *thread, object::Method *method) {
                    InvokeHelper::before_invoke(thread, method);

                    // Method::invoke() only sets thread->pc to its body start
                    // or just invoked a native function
                    method->invoke(thread);
                }


                static inline void invoke_simple(Thread *thread, object::Method *method,
                                                 SizeT locals_size) {
                    // ensure that we can exit successfully.
                    Byte exit_code = static_cast<Byte>(VMOpcodes::halt);

                    // set current pc to exit_code, so that when the method returns,
                    // the program ends.
                    thread->pc = &exit_code;

                    // Create initial frame, just as a data holder.
                    thread->stack.new_frame(locals_size);

                    invoke_method(thread, method);

                    // start interpreting
                    thread->run();

                    // In order to let our's caller be accessible to the return value,
                    // do not call remove_top_frame()
                }


            private:
                /* Outer interfaces to Interpreter */

                static inline void method_return_void(Thread *thread) {
                    // return void, just dispose frame and return!
                    InvokeHelper::return_dispose(thread);
                }

                static inline void method_return_object(Thread *thread) {
                    object::Object *ret = thread->stack.peek_object_pop();
                    InvokeHelper::return_dispose(thread);
                    thread->stack.push_object_ref(ret);
                }

                template <typename T>
                static inline void method_return(Thread *thread) {
                    T ret = thread->stack.peek_pop<T>();
                    InvokeHelper::return_dispose(thread);
                    thread->stack.push<T>(std::forward<T>(ret));
                }

                static inline void invoke_method(Thread *thread) {
                    UInt16 name_id = thread->const_u16();
                    dvm_memory_barrier();
                    UInt16 signature_id = thread->const_u16();

                    object::Method *method =
                            InvokeHelper::resolve_by_id(thread, name_id, signature_id);
                    Dispatcher::invoke_method(thread, method);
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
