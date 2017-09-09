//
// Created by kiva on 2017/9/6.
//

#include "dvm_method.hpp"

namespace dvm {
    namespace core {
        namespace object {

            DvmMethod::DvmMethod(const std::string &name, const std::string &signature, Byte *body, SizeT length)
                    : AbsMethod(name, signature, False), method_body(body), method_length(length) {
            }

            void DvmMethod::invoke(interpreter::vm_context &context) {
            }
        }
    }
}
