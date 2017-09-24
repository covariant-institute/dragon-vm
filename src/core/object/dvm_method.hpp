//
// Created by kiva on 2017/9/6.
//

#pragma once

#include "abs_method.hpp"

namespace dvm {
    namespace core {
        namespace object {
            class DvmMethod : public AbsMethod {
            private:
                Byte *method_body;
                SizeT method_length;

            public:
                DvmMethod(Class *return_type, const std::string &name, const std::string &signature, Bool is_static_method,
                          Byte *body, SizeT length);

                ~DvmMethod() override = default;

                void invoke(runtime::VMContext *context) override;
            };
        }
    }
}
