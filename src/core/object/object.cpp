//
// Created by kiva on 2017/9/15.
//
#include <core/object/object.hpp>
#include <core/memory.hpp>

namespace dvm {
    namespace core {
        namespace object {

            Object *Object::null_object() {
                static Object *object = nullptr;
                if (object == nullptr) {
                    object = (Object *) dvm_malloc(sizeof(Object));
                    object->prototype = nullptr;
                }
                return object;
            }
        }
    }
}
