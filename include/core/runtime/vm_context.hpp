#pragma once

#include <core/object/class.hpp>
#include <core/runtime/dvm.hpp>
#include <core/runtime/thread.hpp>
#include <unordered_map>

namespace dvm {
    namespace core {
        namespace object {
            class Method;
        }

        namespace runtime {
            class Thread;

            class DragonVM;

            // TODO VM Executing Context
            class VMContext {
                friend class DragonVM;

            private:
                DragonVM *dvm;

                VMContext() = default;

                ~VMContext() = default;

            public:
                VMContext(const VMContext &) = delete;

                inline void register_class(const std::string &class_name, object::Class *prototype) {
                    dvm->register_class(class_name, prototype);
                }

                inline object::Class *find_class(const std::string &class_name) const {
                    return dvm->find_class(class_name);
                }

                inline void register_method(const std::string &method_name, const std::string &signature,
                                            object::Method *method) {
                    dvm->register_method(method_name, signature, method);
                }

                inline object::Method *resolve_method(const std::string &method_name,
                                                      const std::string &signature) const {
                    return dvm->resolve_method(method_name, signature);
                }

                inline void register_constant(UInt16 constant_id, const std::string &data) {
                    dvm->register_constant(constant_id, data);
                }

                inline void register_constant(UInt16 constant_id, object::Class *constant) {
                    dvm->register_constant(constant_id, constant);
                };

                inline const std::string &find_constant(UInt16 constant_id) const {
                    return dvm->find_constant(constant_id);
                }

#define CREATOR_SIGNATURE(TYPE) \
            object::Object* new_##TYPE(TYPE value)

                CREATOR_SIGNATURE(Int32);

                CREATOR_SIGNATURE(Int64);

                CREATOR_SIGNATURE(UInt32);

                CREATOR_SIGNATURE(UInt64);

                CREATOR_SIGNATURE(Float);

                CREATOR_SIGNATURE(Double);

                object::Object *null_object();

#undef CREATOR_SIGNATURE
            };
        }
    }
}

