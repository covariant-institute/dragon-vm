//
// Created by kiva on 2017/8/31.
//

#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>
#include <core/object/class.hpp>
#include <core/object/object.hpp>
#include <unordered_map>
#include <cstdlib>
#include <core/memory.hpp>

namespace dvm {
    namespace core {
        namespace object {
            static std::unordered_map<std::string, Class*>& get_global_class_map() {
                static std::unordered_map<std::string, Class*> class_map;
                return class_map;
            };

            static void put_class(const std::string &name, Class* clazz) {
                get_global_class_map().insert(std::unordered_map<std::string, Class*>::value_type(name, clazz));
            }

            static Class* create_class(int class_slot_count) {
                return (Class *) dvm_malloc(sizeof(Class) + sizeof(Slot) * class_slot_count);
            }

            static Class *find_class_non_const(const std::string &name) {
                try {
                    return get_global_class_map().at(name);

                } catch (const std::out_of_range &e) {
                    throw dvm::core::exception(DVM_CLASS_NOT_FOUND);
                }
            }

            const Class *Class::find_class(const std::string &name) {
                return find_class_non_const(name);
            }

            Class *Class::define_bootstrap_class(const std::string &name,
                                                 Int32 class_slot_count, Int32 member_slot_count) {
                Class *clazz = create_class(class_slot_count);
                clazz->type = type_identifier::TYPE_ID_OBJECT;
                clazz->parent = nullptr;
                clazz->name = new std::string(name);
                clazz->class_slot_count = class_slot_count;
                clazz->member_slot_count = member_slot_count;
                put_class(name, clazz);
                return clazz;
            }

            const Class *Class::define_class(Class *parent, const std::string &name,
                                             Int32 class_slot_count, Int32 member_slot_count) {
                Class *parent_class = parent;
                if (parent_class == nullptr) {
                    parent_class = find_class_non_const("Object");
                }

                if (parent_class->type != type_identifier::TYPE_ID_OBJECT
                    || member_slot_count < 1) {
                    throw dvm::core::exception(DVM_INVALID_OBJECT_MEMORY);
                }

                Class *clazz = define_bootstrap_class(name, class_slot_count, member_slot_count);
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
                // Copy prototype reference
                uninitialized->prototype = this;

                // Parent object instantiation
                uninitialized->slots[0].slot_type = type_identifier::TYPE_ID_OBJECT;
                if (this->parent != nullptr) {
                    uninitialized->slots[0].object = this->parent->new_instance();
                } else {
                    uninitialized->slots[0].object = uninitialized;
                }

                return uninitialized;
            }
        }
    }
}
