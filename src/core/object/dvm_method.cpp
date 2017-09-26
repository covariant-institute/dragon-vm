//
// Created by kiva on 2017/9/6.
//

#include "dvm_method.hpp"

namespace dvm {
    namespace core {
        namespace object {
            DvmMethod::DvmMethod(const Class *return_type, const std::string &name, const std::string &signature,
                                 Bool is_static_method, Bool is_native_method, UInt16 frame_size)
                    : Method(return_type,
                             name,
                             signature,
                             is_static_method,
                             is_native_method,
                             frame_size) {

            }

            void DvmMethod::invoke(runtime::Thread *thread) {
                thread->set_runnable(method_body);
                thread->run();
            }
        }
    }
}
