#pragma once

#include <core/memory.hpp>
#include <core/stack.hpp>
#include <core/heap.hpp>
#include <core/object/class.hpp>
#include <core/runtime/opcodes.hpp>
#include <core/runtime/vm_register.hpp>
#include <core/dcx/dcx_file.hpp>

#include <unordered_map>

namespace dvm {
    namespace core {
        namespace runtime {
            // TODO VM Executing Context
            class VMContext {
            private:
                std::unordered_map<std::string, object::Class *> class_map;
                std::shared_ptr<dcx::DcxFile> dcx_file;

                Stack stack;
                VMOpcode *pc;

            public:
                VMContext();

                VMContext(const VMContext &) = delete;

                explicit VMContext(std::shared_ptr<dcx::DcxFile> dcx_file);

                ~VMContext();

                void set_file(const std::shared_ptr<dcx::DcxFile> &dcx_file);

                void register_class(const std::string &class_name, object::Class *prototype);

                object::Class *find_class(const std::string &class_name);
            };
        }
    }
}

