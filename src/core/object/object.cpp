//
// Created by kiva on 2017/9/1.
//

#include <core/object/object.hpp>
#include <core/memory.hpp>

namespace dvm {
    namespace core {
        namespace object {
            Object *Object::create_object(const Class *prototype) {
                if (prototype == nullptr) {
                    return nullptr;
                }

                Object *object = (Object *) dvm_malloc(sizeof(Object) + sizeof(Slot) * prototype->member_slot_count);

                // Copy prototype reference
                object->prototype = prototype;

                // Parent object instantiation
                object->slots[0].slot_type = type_identifier::TYPE_ID_OBJECT;
                if (prototype->parent != nullptr) {
                    object->slots[0].object = prototype->parent->create_object();
                } else {
                    object->slots[0].object = object;
                }

                return object;
            }
        }
    }
}

