//
// Created by kiva on 2017/9/6.
//

#pragma once

#include <core/object/method.hpp>

namespace dvm {
    namespace core {
        namespace object {
            class DvmMethod : public Method {
                friend class Method;

            private:
                Byte *method_body;
                SizeT method_length;

            public:
                DvmMethod(const Class *return_type,
                          const std::string &name,
                          const std::string &signature,
                          Bool is_static_method,
                          Bool is_native_method);

                ~DvmMethod() override = default;

                void invoke(runtime::Thread *thread) override;
            };
        }
    }
}
