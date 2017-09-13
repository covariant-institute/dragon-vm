//
// Created by kiva on 2017/9/6.
//

#include <core/object/method.hpp>
#include "dvm_method.hpp"
#include "native_method.hpp"

namespace dvm {
    namespace core {
        namespace object {
            Method *Method::resolve(runtime::VMContext &context, const std::string &name,
                                    const std::string &signature) {
                return context.resolve_method(name, signature);
            }

            void Method::register_method(runtime::VMContext &context, Class *return_type,
                                            const std::string &name, const std::string &signature,
                                            Bool is_static_method, Byte *body, SizeT length) {
                auto method = new DvmMethod(return_type, name, signature, is_static_method, body, length);
                context.register_method(name, signature, method);
            }

            void
            Method::register_native_method(runtime::VMContext &context, Class *return_type, const std::string &name,
                                           const std::string &signature, Bool is_static_method) {
                auto method = new NativeMethod(return_type, name, signature, is_static_method);
                context.register_method(name, signature, method);
            }
        }
    }
}
