//
// Created by kiva on 2017/9/6.
//

#pragma once

#include "abs_method.hpp"

namespace dvm {
    namespace core {
        namespace object {
            class ForeignMethod : public AbsMethod {
            private:

            public:
                ForeignMethod(const std::string &name, const std::string &signature);

                ~ForeignMethod() override = default;

                void invoke(runtime::VMContext &context) override;
            };
        }
    }
}
