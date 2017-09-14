//
// Created by kiva on 2017/9/13.
//
#include "native_method.hpp"

namespace dvm {
    namespace core {
        namespace object {

            namespace resolver {
                NativeMethodCallable::Type resolve_native(const std::string &name, const std::string &signature) {
                    // TODO find symbol in executable e.g. *.so *.dylib and *.dll
                    return nullptr;
                }
            }

            NativeMethod::NativeMethod(Class *return_type, const std::string &name, const std::string &signature,
                                       Bool is_static_method)
                    : AbsMethod(return_type, name, signature, True, is_static_method) {
            }

            NativeMethod::~NativeMethod() = default;

            object::Object *NativeMethod::invoke(runtime::VMContext &context) {
                return callable(context);
            }
        }
    }
}
