//
// Created by kiva on 2017/9/13.
//
#include "native_method.hpp"
#include <core/platform/dl.hpp>

namespace dvm {
    namespace core {
        namespace object {

            namespace resolver {
                NativeMethodCallable::Type resolve_native(const std::string &name, const std::string &signature) {
                    // TODO Convert method name and signature name to symbol name
                    return (NativeMethodCallable::Type) dl::DLInterface().find_symbol(name);
                }
            }

            NativeMethod::NativeMethod(const Class *return_type, const std::string &name, const std::string &signature,
                                       Bool is_static_method, Bool is_native_method, UInt16 frame_size)
                    : Method(return_type, name, signature, is_static_method, is_native_method, frame_size) {

            }

            void NativeMethod::invoke(runtime::Thread *thread) {
                if (callable.callable == nullptr) {
                    callable.callable = resolver::resolve_native(get_name(), get_signature());
                }

                callable(thread);
            }
        }
    }
}
