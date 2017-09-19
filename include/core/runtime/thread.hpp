//
// Created by kiva on 2017/9/13.
//

#pragma once

#include <core/stack.hpp>
#include "opcodes.hpp"

namespace dvm {
    namespace core {
        namespace runtime {
            class Runnable {
            public:
                virtual void run() = 0;
            };

            /**
             * TODO: Thread and Thread Pool
             */
            class Thread {
                friend class Interpreter;

            private:
                VMOpcode *pc;
                Stack stack;

            public:
                Thread();

                Thread(const Thread &) = delete;

                ~Thread();

                void set_runnable(Byte *code);
            };
        }
    }
}

