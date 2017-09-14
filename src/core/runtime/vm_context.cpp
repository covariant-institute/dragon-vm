//
// Created by kiva on 2017/9/6.
//

#include <core/runtime/vm_context.hpp>
#include <core/object/method.hpp>
#include <core/dcx/dcx_linker.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            static void vm_init_classes(VMContext &context) {
                using namespace dvm::core::object;
                Class *object = Class::define_bootstrap_class(context, "Object", 0, 1);
                Class::define_class(context, object, "Int8", 0, 2);
                Class::define_class(context, object, "Int16", 0, 2);
                Class::define_class(context, object, "Int32", 0, 2);
                Class::define_class(context, object, "Int64", 0, 2);
                Class::define_class(context, object, "UInt8", 0, 2);
                Class::define_class(context, object, "UInt16", 0, 2);
                Class::define_class(context, object, "UInt32", 0, 2);
                Class::define_class(context, object, "UInt64", 0, 2);
                Class::define_class(context, object, "Bool", 0, 2);
                Class::define_class(context, object, "Byte", 0, 2);
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
                    throw dvm::core::exception(DVM_RUNTIME_CLASS_NOT_FOUND);
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
                    throw dvm::core::exception(DVM_RUNTIME_METHOD_NOT_FOUND);
                }
            }

            void VMContext::register_constant(UInt32 constant_id, const std::string &data) {
                constant_map[constant_id] = data;
            }

            const std::string &VMContext::find_constant(UInt32 constant_id) const {
                try {
                    return constant_map.at(constant_id);

                } catch (const std::out_of_range &e) {
                    throw dvm::core::exception(DVM_RUNTIME_CONSTANT_NOT_FOUND);
                }
            }

#define CREATOR_SIGNATURE(TYPE) \
            object::Object* VMContext::new_##TYPE(TYPE value)

#define CREATOR_COMMON_IMPLEMENT(TYPE, SLOT_SETTER) \
            CREATOR_SIGNATURE(TYPE) { \
                object::Object *object = this->find_class(#TYPE)->new_instance(); \
                object::ensure_object_valid(object); \
                object->slots[1].set_##SLOT_SETTER(value); \
                return object; \
            }

            CREATOR_COMMON_IMPLEMENT(Int8, i8);

            CREATOR_COMMON_IMPLEMENT(Int16, i16);

            CREATOR_COMMON_IMPLEMENT(Int32, i32);

            CREATOR_COMMON_IMPLEMENT(Int64, i64);

            CREATOR_COMMON_IMPLEMENT(UInt8, u8);

            CREATOR_COMMON_IMPLEMENT(UInt16, u16);

            CREATOR_COMMON_IMPLEMENT(UInt32, u32);

            CREATOR_COMMON_IMPLEMENT(UInt64, u64);

            CREATOR_COMMON_IMPLEMENT(Float, f32);

            CREATOR_COMMON_IMPLEMENT(Double, f64);

            CREATOR_COMMON_IMPLEMENT(Byte, byte);

            CREATOR_COMMON_IMPLEMENT(Bool, bool);

#undef CREATOR_COMMON_IMPLEMENT

#undef CREATOR_SIGNATURE
        }
    }
}
