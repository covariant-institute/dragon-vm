//
// Created by kiva on 2017/9/13.
//

#pragma once

#include <core/stack.hpp>
#include <core/runtime/opcodes.hpp>
#include <core/runtime/interpreter.hpp>

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
                friend class VMContext;

            private:
                Interpreter interpreter;
                VMOpcode *pc;
                Stack stack;

                void run_with_context(VMContext *context);

                /* Utility functions for pc */
                inline UInt8 const_i8() {
                    return *pc++;
                }

                inline UInt16 const_i16() {
                    UInt8 h = const_i8();
                    dvm_memory_barrier();
                    UInt8 l = const_i8();
                    return h << 8 + l;
                }

                inline Int32 const_i32() {
                    UInt8 byte1 = const_i8();
                    dvm_memory_barrier();

                    UInt8 byte2 = const_i8();
                    dvm_memory_barrier();

                    UInt8 byte3 = const_i8();
                    dvm_memory_barrier();

                    UInt8 byte4 = const_i8();
                    return byte1 << 24 + byte2 << 16 + byte3 << 8 + byte4;
                }

            public:
                Thread();

                Thread(const Thread &) = delete;

                ~Thread();

                void set_runnable(Byte *code);
            };
        }
    }
}

