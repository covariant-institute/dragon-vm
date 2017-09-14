//
// Created by kiva on 2017/9/13.
//
#pragma once

#include "abs_method.hpp"

namespace dvm {
    namespace core {
        namespace object {
            struct NativeMethodCallable {
                using Type = object::Object *(*)(runtime::VMContext &context);

                Type callable;

                object::Object *operator()(runtime::VMContext &context) {
                    if (callable != nullptr) {
                        return callable(context);
                    }
                    return context.null_object();
                }
            };

            class NativeMethod : public AbsMethod {
            private:
                NativeMethodCallable callable;

            public:
                NativeMethod(Class *return_type, const std::string &name, const std::string &signature,
                             Bool is_static_method);

                ~NativeMethod() override;

                object::Object *invoke(runtime::VMContext &context) override;
            };
        }
    }
}
