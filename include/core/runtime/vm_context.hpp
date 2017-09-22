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
            class Thread;

            // TODO VM Executing Context
            class VMContext {
            private:
                std::unordered_map<std::string, object::Class *> class_map;
                std::unordered_map<std::string, std::unordered_map<std::string, object::Method *>> method_map;
                std::unordered_map<UInt16, std::string> constant_string_map;
                std::unordered_map<UInt16, object::Class *> constant_class_map;

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

                void register_constant(UInt16 constant_id, const std::string &data);

                void register_constant(UInt16 constant_id, object::Class *constant);

                const std::string &find_constant(UInt16 constant_id) const;

                void run_thread(Thread *thread);

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

