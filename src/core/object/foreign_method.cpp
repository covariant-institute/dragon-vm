//
// Created by kiva on 2017/9/6.
//

#include "foreign_method.hpp"

namespace dvm {
    namespace core {
        namespace object {

            ForeignMethod::ForeignMethod(const std::string &name, const std::string &signature, FFIFunction ffi)
                    : AbsMethod(name, signature, True), foreign_function(ffi) {
            }

            void ForeignMethod::invoke(runtime::VMContext &context) {
                if (foreign_function != nullptr) {
                    foreign_function(context);
                }
            }
        }
    }
}
