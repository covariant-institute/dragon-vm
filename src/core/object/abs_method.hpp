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
                Class *return_type;
                Bool is_native_method;
                Bool is_static_method;

            public:
                AbsMethod(Class *return_type, const std::string &name, const std::string &signature,
                          Bool is_native_method, Bool is_static_method);

                virtual ~AbsMethod() = default;

                Bool is_native() override;

                Bool is_static() override;

                Class *get_return_type() override;

                const std::string &get_signature() override;

                const std::string &get_name() override;

                void invoke(runtime::Thread *thread) override = 0;
            };
        }
    }
}
