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

                template <typename T>
                static inline void store(Thread *thread) {
                    UInt8 reg = thread->const_i8();

                    // Do not use peek_pop()
                    // st_* instructions do not drop stack elements
                    thread->regs.get_register(reg)->set_unchecked(thread->stack.peek<T>());
                }

                static inline void store_object_ref(Thread *thread) {
                    UInt8 reg = thread->const_i8();

                    // Do not use peek_object_pop()
                    // st_* instructions do not drop stack elements
                    thread->regs.get_register(reg)->set_unchecked(thread->stack.peek_object());
                }

                template <typename T>
                static inline void load(Thread *thread) {
                    UInt8 reg = thread->const_i8();
                    thread->stack.push(
                            thread->regs.get_register(reg)->get_unchecked<T>());
                }

                static inline void load_object_ref(Thread *thread) {
                    UInt8 reg = thread->const_i8();
                    thread->stack.push_object_ref(
                            thread->regs.get_register(reg)->get_unchecked<object::Object *>());
                }
            };
        }
    }
}
