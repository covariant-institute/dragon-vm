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

            bool Object::is_array() const {
                return false;
            }

            Object::Object(const Class *prototype) : prototype(prototype) {
            }

            void Object::init() {
                this->prototype = prototype;

                // Parent object instantiation
                if (prototype->parent != nullptr) {
                    this->slots[0].set(prototype->parent->new_instance());
                } else {
                    this->slots[0].set(this);
                }
            }
        }
    }
}
