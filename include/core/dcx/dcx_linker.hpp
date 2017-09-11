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
            public:
                static void validate(runtime::VMContext &context, std::shared_ptr<DcxFile> dcx_file);

                static void link(runtime::VMContext &context, std::shared_ptr<DcxFile> dcx_file);
            };
        }
    }
}

