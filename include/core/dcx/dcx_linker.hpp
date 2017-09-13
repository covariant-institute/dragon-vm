//
// Created by kiva on 2017/9/11.
//
#pragma once

#include <core/dcx/dcx_file.hpp>
#include <core/runtime/vm_context.hpp>

namespace dvm {
    namespace core {
        namespace dcx {
            class DcxLinker {
            private:
                static std::string constant_to_string(std::shared_ptr<DcxFile> dcx_file, UInt32 constant_id);

                static void validate_class(DcxFileClassEntry &entry);

                static void validate_method(DcxFileMethodEntry &entry);

                static void link_class(runtime::VMContext &context, std::shared_ptr<DcxFile> dcx_file);

                static void link_method(runtime::VMContext &context, std::shared_ptr<DcxFile> dcx_file);

            public:

                static void link(runtime::VMContext &context, std::shared_ptr<DcxFile> dcx_file);
            };
        }
    }
}

