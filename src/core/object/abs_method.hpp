//
// Created by kiva on 2017/9/6.
//

#pragma once

#include <core/object/method.hpp>

namespace dvm {
    namespace core {
        namespace object {
            class AbsMethod : public Method {
            private:
                std::string method_name;
                std::string method_signature;
                Bool is_foreign;

            public:
                AbsMethod(const std::string &name, const std::string &signature, Bool is_foreign);

                virtual ~AbsMethod() = default;

                Bool is_foreign_method() override;

                const std::string &get_signature() override;

                const std::string &get_name() override;

                void invoke(runtime::VMContext &context) override = 0;
            };
        }
    }
}
