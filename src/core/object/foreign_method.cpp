//
// Created by kiva on 2017/9/6.
//

#include "foreign_method.hpp"

namespace dvm {
    namespace core {
        namespace object {

            ForeignMethod::ForeignMethod(const std::string &name, const std::string &signature)
                    : AbsMethod(name, signature, True) {
            }

            void ForeignMethod::invoke(runtime::VMContext &context) {
            }
        }
    }
}
