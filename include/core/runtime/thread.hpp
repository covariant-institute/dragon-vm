//
// Created by kiva on 2017/9/13.
//

#pragma once

#include <core/stack.hpp>
#include <core/runtime/opcodes.hpp>
#include <core/runtime/interpreter.hpp>
#include <core/runtime/vm_register.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            class VMContext;

            class ThreadLocalStorage {

            };

            /**
             * TODO: Thread and Thread Pool
             */
            class Thread {
                friend class Interpreter;

                friend class Utils;

                friend class VMContext;

                friend class DragonVM;

            private:
                Interpreter interpreter;
                Stack stack;
                VMRegisterHolder regs;

                VMContext *context;
                Byte *code_base;
                VMOpcode *pc;

                explicit Thread(VMContext *context);

                /* Utility functions for pc */
                inline UInt8 const_u8() {
                    return *pc++;
                }

                inline UInt16 const_u16() {
                    UInt8 &&h = const_u8();
                    dvm_memory_barrier();

                    UInt8 &&l = const_u8();
                    return (h << 8) + l;
                }

                inline Int16 const_i16() {
                    UInt8 &&h = const_u8();
                    dvm_memory_barrier();

                    UInt8 &&l = const_u8();
                    return (h << 8) + l;
                }

                inline Int32 const_i32() {
                    UInt16 &&h = const_u16();
                    dvm_memory_barrier();

                    UInt16 &&l = const_u16();

                    return (h << 16) + l;
                }

                inline Float const_f32() {
                    union {
                        Float f;
                        UInt8 bits[sizeof(Float)];
                    } c{ };

                    c.bits[0] = const_u8();
                    dvm_memory_barrier();

                    c.bits[1] = const_u8();
                    dvm_memory_barrier();

                    c.bits[2] = const_u8();
                    dvm_memory_barrier();

                    c.bits[3] = const_u8();
                    return c.f;
                }

            public:
                Thread(const Thread &) = delete;

                ~Thread() = default;

                void set_runnable(Byte *code);

                void run();

                inline VMContext *get_context() const {
                    return context;
                }

                inline Stack &get_stack() {
                    return stack;
                }

                inline VMRegisterHolder &get_registers() {
                    return regs;
                }
            };
        }
    }
}

