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
                const Class *return_type;
                Bool is_native_method;
                Bool is_static_method;

            public:
                AbsMethod(const Class *return_type, const std::string &name, const std::string &signature,
                          Bool is_native_method, Bool is_static_method);

                virtual ~AbsMethod() = default;

                Bool is_native() const override;

                Bool is_static() const override;

                const Class *get_return_type() const override;

                const std::string &get_signature() const override;

                const std::string &get_name() const override;

                void invoke(runtime::Thread *thread) override = 0;
            };
        }
    }
}
