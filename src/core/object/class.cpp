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

            static Class* create_class() {
                return (Class *) dvm_malloc(sizeof(Class));
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

            Class *Class::define_bootstrap_class(const std::string &name, int slot_count) {
                Class *clazz = create_class();
                clazz->type = type_identifier::TYPE_ID_OBJECT;
                clazz->parent = nullptr;
                clazz->name = name;
                clazz->slot_count = slot_count;
                put_class(name, clazz);
                return clazz;
            }

            const Class *Class::define_class(Class *parent, const std::string &name, int slot_count) {
                Class *parent_class = parent;
                if (parent_class == nullptr) {
                    parent_class = find_class_non_const("Object");
                }

                if (parent_class->type != type_identifier::TYPE_ID_OBJECT
                    || slot_count < 1) {
                    throw dvm::core::exception(DVM_INVALID_OBJECT_MEMORY);
                }

                Class *clazz = define_bootstrap_class(name, slot_count);
                clazz->parent = parent_class;
                return clazz;
            }

            Object *Class::create_object() const {
                return Object::create_object(this);
            }
        }
    }
}
