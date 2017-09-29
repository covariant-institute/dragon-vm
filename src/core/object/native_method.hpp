//
// Created by kiva on 2017/9/13.
//
#pragma once

#include <core/object/method.hpp>

namespace dvm {
    namespace core {
        namespace object {
            struct NativeMethodCallable {
                using Type = void (*)(runtime::Thread *thread);

                Type callable;

                void operator()(runtime::Thread *thread) {
                    if (callable != nullptr) {
                        callable(thread);
                    }
                }
            };

            class NativeMethod : public Method {
            private:
                NativeMethodCallable callable;

            public:
                NativeMethod(const Class *return_type,
                             const std::string &name,
                             const std::string &signature,
                             Bool is_static_method,
                             Bool is_native_method);

                ~NativeMethod() override = default;

                void invoke(runtime::Thread *thread) override;
            };
        }
    }
}
