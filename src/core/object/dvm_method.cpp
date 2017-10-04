//
// Created by kiva on 2017/9/6.
//

#include "dvm_method.hpp"

namespace dvm {
    namespace core {
        namespace object {
            DvmMethod::DvmMethod(const Class *return_type, const std::string &name, const std::string &signature,
                                 Bool is_static_method, Bool is_native_method)
                    : Method(return_type,
                             name,
                             signature,
                             is_static_method,
                             is_native_method) {

            }

            void DvmMethod::prepare(runtime::Thread *thread) {
                thread->set_runnable(method_body);
            }

            void DvmMethod::dump() const {
                printf("  Method Data:\n");
                printf("    Body size:  %llu\n", method_length);
                printf("    Body start: %p\n", static_cast<void *>(method_body));
            }
        }
    }
}
