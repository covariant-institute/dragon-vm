//
// Created by kiva on 2017/9/22.
//
#pragma once

#include <core/runtime/interpreter.hpp>
#include <core/runtime/thread.hpp>
#include <core/runtime/vm_register.hpp>

namespace dvm {
    namespace core {
        namespace runtime {

            class Utils {
            public:
                template <typename FromType, typename ToType>
                static inline void cast(Thread *thread) {
                    FromType &&from = std::move(thread->stack.peek_pop<FromType>());
                    thread->stack.push<ToType>(std::forward<ToType>(from));
                }

                template <typename LhsType, typename Impl, typename RhsType = LhsType>
                static inline void math(Thread *thread) {
                    LhsType &&lhs = thread->stack.peek_pop<LhsType>();
                    dvm_memory_barrier();
                    RhsType &&rhs = thread->stack.peek_pop<RhsType>();

                    LhsType &&result = Impl::get_result(std::forward<LhsType>(lhs),
                                                        std::forward<RhsType>(rhs));
                    thread->stack.push<LhsType>(std::forward<LhsType>(result));
                }

                template <typename LhsType, typename Impl>
                static inline void math1(Thread *thread) {
                    LhsType &&operand = thread->stack.peek_pop<LhsType>();

                    LhsType &&result = Impl::get_result(std::forward<LhsType>(operand));
                    thread->stack.push<LhsType>(std::forward<LhsType>(result));
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
