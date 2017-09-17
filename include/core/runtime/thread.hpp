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
            public:
            };

            class VMThread : public Thread {
            private:
                Stack thread_stack;
            };
        }
    }
}

