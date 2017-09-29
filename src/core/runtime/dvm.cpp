//
// Created by kiva on 2017/9/24.
//

#include <core/object/class.hpp>
#include <core/runtime/dvm.hpp>
#include <core/runtime/thread.hpp>
#include <core/runtime/context.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            static void vm_boot(VMContext *context) {
                using namespace dvm::core::object;
                Class *object = Class::define_bootstrap_class(context, "Object", 0, 1);
                Class::define_class(context, object, "Void", 0, 1);
                Class::define_class(context, object, "Int8", 0, 2);
                Class::define_class(context, object, "Int16", 0, 2);
                Class::define_class(context, object, "Int32", 0, 2);
                Class::define_class(context, object, "Int64", 0, 2);
                Class::define_class(context, object, "UInt8", 0, 2);
                Class::define_class(context, object, "UInt16", 0, 2);
                Class::define_class(context, object, "UInt32", 0, 2);
                Class::define_class(context, object, "UInt64", 0, 2);
                Class::define_class(context, object, "Float", 0, 2);
                Class::define_class(context, object, "Double", 0, 2);
            }

            DragonVM::DragonVM() {
                bootstrapContext = new VMContext;
                bootstrapContext->dvm = this;
                bootstrapThread = new Thread(bootstrapContext);
                vm_boot(bootstrapContext);
            }

            DragonVM::~DragonVM() {
                // TODO release resources
                delete bootstrapContext;
                delete bootstrapThread;
            }

            void DragonVM::register_class(const std::string &class_name, object::Class *prototype) {
                class_map[class_name] = prototype;
            }

            object::Class *DragonVM::find_class(const std::string &class_name) const {
                try {
                    return class_map.at(class_name);

                } catch (const std::out_of_range &e) {
                    throw dvm::core::Exception(DVM_RUNTIME_CLASS_NOT_FOUND);
                }
            }

            void DragonVM::register_method(const std::string &method_name, const std::string &signature,
                                            object::Method *method) {
                method_map[method_name][signature] = method;
            }

            object::Method *DragonVM::resolve_method(const std::string &method_name,
                                                      const std::string &signature) const {
                try {
                    return method_map.at(method_name).at(signature);

                } catch (const std::out_of_range &e) {
                    throw dvm::core::Exception(DVM_RUNTIME_METHOD_NOT_FOUND);
                }
            }

            void DragonVM::register_constant(UInt16 constant_id, const std::string &data) {
                constant_string_map[constant_id] = data;
            }

            void DragonVM::register_constant(UInt16 constant_id, object::Class *constant) {
                constant_class_map[constant_id] = constant;
            };

            const std::string &DragonVM::find_constant(UInt16 constant_id) const {
                try {
                    return constant_string_map.at(constant_id);

                } catch (const std::out_of_range &e) {
                    throw dvm::core::Exception(DVM_RUNTIME_CONSTANT_NOT_FOUND);
                }
            }

            const object::Class *DragonVM::find_class_constant(UInt16 constant_id) {
                try {
                    return constant_class_map.at(constant_id);

                } catch (const std::out_of_range &e) {
                    throw dvm::core::Exception(DVM_RUNTIME_CLASS_NOT_FOUND);
                }
            }

            Thread *DragonVM::current_thread() {
                return bootstrapThread;
            }

            void DragonVM::attachCurrentThread() {
            }

            void DragonVM::detachCurrentThread() {
            }
        }
    }
}
