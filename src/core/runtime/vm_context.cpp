//
// Created by kiva on 2017/9/6.
//

#include <core/runtime/vm_context.hpp>
#include <core/runtime/thread.hpp>

namespace dvm {
    namespace core {
        namespace runtime {

            void VMContext::run_thread(Thread *thread) {
                if (thread != nullptr) {
                    thread->run_with_context(this);
                }
            }

#define CREATOR_SIGNATURE(TYPE) \
            object::Object* VMContext::new_##TYPE(TYPE value)

#define CREATOR_COMMON_IMPLEMENT(TYPE) \
            CREATOR_SIGNATURE(TYPE) { \
                object::Object *object = this->find_class(#TYPE)->new_instance(); \
                object::ensure_object_valid(object); \
                object->slots[1].set<TYPE>(value); \
                return object; \
            }

            CREATOR_COMMON_IMPLEMENT(Int32);

            CREATOR_COMMON_IMPLEMENT(Int64);

            CREATOR_COMMON_IMPLEMENT(UInt32);

            CREATOR_COMMON_IMPLEMENT(UInt64);

            CREATOR_COMMON_IMPLEMENT(Float);

            CREATOR_COMMON_IMPLEMENT(Double);

            object::Object *VMContext::null_object() {
                return object::Object::null_object();
            }

#undef CREATOR_COMMON_IMPLEMENT

#undef CREATOR_SIGNATURE
        }
    }
}
