//
// Created by kiva on 2017/10/15.
//

#pragma once

#include <core/object/object.hpp>
#include <core/runtime/context.hpp>

namespace dvm {
    namespace core {
        namespace object {

#define BOXER_GENERATOR(T) \
            object::Object *box(runtime::VMContext *context, T &&type) { \
                return context->new_##T(std::forward<T>(type)); \
            }

            BOXER_GENERATOR(Int32);

            BOXER_GENERATOR(Int64);

            BOXER_GENERATOR(UInt32);

            BOXER_GENERATOR(UInt64);

            BOXER_GENERATOR(Float);

            BOXER_GENERATOR(Double);

#undef BOXER_GENERATOR

            template <typename T>
            T unbox(object::Object *object) {
                ensure_object_available(object);
                if (object->get_prototype()->is_primitive()) {
                    return object->get_slot(1)->get<T>();
                }
                // TODO Abort
                return T();
            }
        }
    }
}
