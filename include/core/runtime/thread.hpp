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

            class Runnable {
            public:
                virtual void run() = 0;
            };

            /**
             * TODO: Thread and Thread Pool
             */
            class Thread {
                friend class Interpreter;

                friend class Utils;

                friend class VMContext;

            private:
                Interpreter interpreter;
                VMOpcode *pc;
                VMRegisterHolder regs;
                Stack stack;

                void run_with_context(VMContext *context);

                /* Utility functions for pc */
                inline UInt8 const_i8() {
                    return *pc++;
                }

                inline Int16 const_i16() {
                    UInt8 &&h = const_i8();
                    dvm_memory_barrier();

                    UInt8 &&l = const_i8();
                    return (h << 8) + l;
                }

                inline Int32 const_i32() {
                    Int16 &&h = const_i16();
                    dvm_memory_barrier();

                    Int16 &&l = const_i16();

                    return (h << 16) + l;
                }

                inline Float const_f32() {
                    union {
                        Float f;
                        UInt8 bits[sizeof(Float)];
                    } c{ };

                    c.bits[0] = const_i8();
                    dvm_memory_barrier();

                    c.bits[1] = const_i8();
                    dvm_memory_barrier();

                    c.bits[2] = const_i8();
                    dvm_memory_barrier();

                    c.bits[3] = const_i8();
                    return c.f;
                }

            public:
                Thread();

                Thread(const Thread &) = delete;

                ~Thread();

                void set_runnable(Byte *code);

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

