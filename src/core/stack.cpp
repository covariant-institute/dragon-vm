//
// Created by kiva on 2017/9/10.
//

#include <core/stack.hpp>

namespace dvm {
    namespace core {

        Stack::~Stack() {
            dvm_free(memory);
        }

        Stack::Stack(SizeT size) : memory(dvm_malloc(size)) {
            sp = reinterpret_cast<Byte *>(memory) + size;
            sl = sp;
            bp = sp;
        }

        Byte *Stack::allocate_on_stack(SizeT size) {
            if (sp - reinterpret_cast<Byte *>(memory) < size + sizeof(SizeT)) {
                throw dvm::core::exception(DVM_MEMORY_STACK_OVERFLOW);
            }

            sp -= size + sizeof(SizeT);
            *reinterpret_cast<SizeT *>(sp) = size;
            return sp + sizeof(SizeT);
        }

//        object::Object *Stack::new_object(const object::Class *prototype) {
//            if (prototype == nullptr) {
//                return object::Object::null_object();
//            }
//
//            auto *uninitialized = allocate_on_stack(prototype->calculate_needed_size());
//            return prototype->new_instance(reinterpret_cast<object::Object *>(uninitialized));
//        }
    }
}
