//
// Created by kiva on 2017/9/6.
//

#include <core/runtime/context.hpp>
#include <core/runtime/thread.hpp>

namespace dvm {
    namespace core {
        namespace runtime {

#define CREATOR_SIGNATURE(T) \
            object::Object* VMContext::new_##T(T &&value)

#define CREATOR_COMMON_IMPLEMENT(T) \
            CREATOR_SIGNATURE(T) { \
                object::Object *object = this->find_class(#T)->new_instance(); \
                object::ensure_object_valid(object); \
                object->slots[1].set<T>(std::forward<T>(value)); \
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
