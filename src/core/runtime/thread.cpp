//
// Created by kiva on 2017/9/19.
//

#include <core/runtime/thread.hpp>
#include <core/config.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            Thread::Thread() :stack(config::STACK_DEFAULT_SIZE), pc(nullptr) {
            }

            Thread::~Thread() = default;

            void Thread::set_runnable(Byte *code) {
                this->pc = code;
                this->code_base = code;
            }

            void Thread::run_with_context(VMContext *context) {
                interpreter.exec(this, context);
            }
        }
    }
}
