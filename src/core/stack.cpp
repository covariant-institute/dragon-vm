//
// Created by kiva on 2017/9/10.
//

#include <core/stack.hpp>

namespace dvm {
    namespace core {

        Stack::~Stack() {
            dvm_free(ss);
        }

        Stack::Stack(SizeT size) : ss(dvm_malloc(size)) {
            sp = reinterpret_cast<Byte *>(ss) + size;
            sl = sp;
        }

        void *dvm::core::Stack::top() {
            ensure_stack_not_empty();
            return reinterpret_cast<void *>(sp + sizeof(SizeT));
        }

        void *Stack::adjust(SizeT size) {
            if (sp - reinterpret_cast<Byte *>(ss) < size + sizeof(SizeT)) {
                throw dvm::core::exception(DVM_MEMORY_STACK_OVERFLOW);
            }

            sp -= size + sizeof(SizeT);
            *reinterpret_cast<SizeT *>(sp) = size;
            return reinterpret_cast<void *>(sp + sizeof(SizeT));
        }

        void Stack::pop() {
            ensure_stack_not_empty();
            sp += *reinterpret_cast<SizeT *>(sp) + sizeof(SizeT);
        }
    }
}
