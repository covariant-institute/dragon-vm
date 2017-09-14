#pragma once

#include <core/memory.hpp>
#include <core/stack.hpp>
#include <core/heap.hpp>
#include <core/object/class.hpp>
#include <core/dcx/dcx_file.hpp>
#include <core/runtime/opcodes.hpp>
#include <core/runtime/vm_register.hpp>

#include <unordered_map>

namespace dvm {
    namespace core {
        namespace object {
            class Method;
        }

        namespace runtime {
            // TODO VM Executing Context
            class VMContext {
            private:
                std::unordered_map<std::string, object::Class *> class_map;
                std::unordered_map<std::string, std::unordered_map<std::string, object::Method *>> method_map;
                std::unordered_map<UInt32, std::string> constant_map;

            public:
                VMContext();

                explicit VMContext(std::shared_ptr<dcx::DcxFile> dcx_file);

                VMContext(const VMContext &) = delete;

                ~VMContext();

                void register_class(const std::string &class_name, object::Class *prototype);

                object::Class *find_class(const std::string &class_name) const;

                void register_method(const std::string &method_name, const std::string &signature,
                                     object::Method *method);

                object::Method *resolve_method(const std::string &method_name,
                                               const std::string &signature) const;

                void register_constant(UInt32 constant_id, const std::string &data);

                const std::string &find_constant(UInt32 constant_id) const;

#define CREATOR_SIGNATURE(TYPE) \
            object::Object* new_##TYPE(TYPE value)

                CREATOR_SIGNATURE(Int8);

                CREATOR_SIGNATURE(Int16);

                CREATOR_SIGNATURE(Int32);

                CREATOR_SIGNATURE(Int64);

                CREATOR_SIGNATURE(UInt8);

                CREATOR_SIGNATURE(UInt16);

                CREATOR_SIGNATURE(UInt32);

                CREATOR_SIGNATURE(UInt64);

                CREATOR_SIGNATURE(Float);

                CREATOR_SIGNATURE(Double);

                CREATOR_SIGNATURE(Byte);

                CREATOR_SIGNATURE(Bool);

                object::Object *null_object();

#undef CREATOR_SIGNATURE
            };
        }
    }
}

