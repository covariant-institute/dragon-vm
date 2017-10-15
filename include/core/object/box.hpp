//
// Created by kiva on 2017/10/15.
//

#pragma once

#include <core/object/object.hpp>

namespace dvm {
    namespace core {
        namespace object {

            template <typename T>
            object::Object *box(T &&value) {
                // TODO auto box object
            }

            template <typename T>
            T unbox(object::Object *object) {
                ensure_object_available(object);
                if (object->prototype->is_primitive()) {
                    return object->slots[1].get<T>();
                }
                // TODO Abort
                return T();
            }
        }
    }
}
