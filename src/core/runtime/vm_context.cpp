//
// Created by kiva on 2017/9/6.
//

#include <core/runtime/vm_context.hpp>
#include <core/runtime/thread.hpp>
#include <core/object/method.hpp>
#include <core/dcx/dcx_linker.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            static void vm_init_classes(VMContext &context) {
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

            static void vm_init(VMContext &context) {
                vm_init_classes(context);
            }

            VMContext::VMContext() {
                vm_init(*this);
            }

            VMContext::VMContext(std::shared_ptr<dcx::DcxFile> dcx_file)
                    : VMContext() {
                dcx::DcxLinker::link(*this, dcx_file);
            }

            VMContext::~VMContext() = default;

            void VMContext::register_class(const std::string &class_name, object::Class *prototype) {
                class_map[class_name] = prototype;
            }

            object::Class *VMContext::find_class(const std::string &class_name) const {
                try {
                    return class_map.at(class_name);

                } catch (const std::out_of_range &e) {
                    throw dvm::core::Exception(DVM_RUNTIME_CLASS_NOT_FOUND);
                }
            }

            void VMContext::register_method(const std::string &method_name, const std::string &signature,
                                            object::Method *method) {
                method_map[method_name][signature] = method;
            }

            object::Method *VMContext::resolve_method(const std::string &method_name,
                                                      const std::string &signature) const {
                try {
                    return method_map.at(method_name).at(signature);

                } catch (const std::out_of_range &e) {
                    throw dvm::core::Exception(DVM_RUNTIME_METHOD_NOT_FOUND);
                }
            }

            void VMContext::register_constant(UInt16 constant_id, const std::string &data) {
                constant_string_map[constant_id] = data;
            }

            void VMContext::register_constant(UInt16 constant_id, object::Class *constant) {
                constant_class_map[constant_id] = constant;
            };

            const std::string &VMContext::find_constant(UInt16 constant_id) const {
                try {
                    return constant_string_map.at(constant_id);

                } catch (const std::out_of_range &e) {
                    throw dvm::core::Exception(DVM_RUNTIME_CONSTANT_NOT_FOUND);
                }
            }


            void VMContext::run_thread(Thread *thread) {
                if (thread != nullptr) {
                    thread->run_with_context(this);
                }
            }

#define CREATOR_SIGNATURE(TYPE) \
            object::Object* VMContext::new_##TYPE(TYPE value)

#define CREATOR_COMMON_IMPLEMENT(TYPE) \
            CREATOR_SIGNATURE(TYPE) { \
                object::Object *object = this->find_class(#TYPE)->new_instance(); \
                object::ensure_object_valid(object); \
                object->slots[1].set<TYPE>(value); \
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
