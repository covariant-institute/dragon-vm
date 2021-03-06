//
// Created by kiva on 2017/8/31.
//

#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>
#include <core/object/class.hpp>
#include <core/object/object.hpp>
#include <core/runtime/context.hpp>
#include <core/memory.hpp>
#include <cstring>

namespace dvm {
    namespace core {
        namespace object {
            static Class* create_class(int class_slot_count) {
                return (Class *) dvm_malloc(sizeof(Class) + sizeof(Slot) * class_slot_count);
            }

            Class *Class::find_class(runtime::VMContext *context, const std::string &name) {
                return context->find_class(name);
            }

            Class *Class::define_bootstrap_class(runtime::VMContext *context, const std::string &name,
                                                 UInt32 class_slot_count, UInt32 member_slot_count) {
                Class *clazz = create_class(class_slot_count);
                clazz->type = TypeIdentifier::TYPE_ID_OBJECT;
                clazz->parent = nullptr;
                clazz->name = new std::string(name);
                clazz->class_slot_count = class_slot_count;
                clazz->member_slot_count = member_slot_count;
                context->register_class(name, clazz);
                return clazz;
            }

            Class *Class::define_class(runtime::VMContext *context, const Class *parent, const std::string &name,
                                             UInt32 class_slot_count, UInt32 member_slot_count) {
                const Class *parent_class = parent;
                if (parent_class == nullptr) {
                    parent_class = context->find_class("Object");
                }

                if (parent_class->type != TypeIdentifier::TYPE_ID_OBJECT
                    || member_slot_count < 1) {
                    throw dvm::core::Exception(DVM_INVALID_OBJECT_MEMORY);
                }

                Class *clazz = define_bootstrap_class(context, name, class_slot_count, member_slot_count);
                clazz->parent = parent_class;
                return clazz;
            }

            Object *Class::new_instance() const {
                return new_instance((Object *) dvm_malloc(calculate_needed_size()));
            }

            SizeT Class::calculate_needed_size() const {
                return sizeof(Object) + sizeof(Slot) * this->member_slot_count;
            }

            Object *Class::new_instance(Object *uninitialized) const {
                // clear memory
                memset(reinterpret_cast<void *>(uninitialized), '\0', calculate_needed_size());

                auto *target = new (uninitialized) Object(this);
                target->init();
                return uninitialized;
            }
        }
    }
}
