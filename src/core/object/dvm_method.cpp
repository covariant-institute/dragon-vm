//
// Created by kiva on 2017/9/6.
//

#include "dvm_method.hpp"

namespace dvm {
    namespace core {
        namespace object {

            DvmMethod::DvmMethod(Class *return_type, const std::string &name, const std::string &signature,
                                 Bool is_static_method, Byte *body, SizeT length)
                    : AbsMethod(return_type, name, signature, False, is_static_method), method_body(body),
                      method_length(length) {
            }

            void DvmMethod::invoke(runtime::VMContext *context) {
            }
        }
    }
}
