//
// Created by kiva on 2017/9/6.
//

#include "abs_method.hpp"


namespace dvm {
    namespace core {
        namespace object {
            AbsMethod::AbsMethod(const std::string &name, const std::string &signature, Bool is_foreign)
                    : method_name(name), method_signature(signature), is_foreign(is_foreign) {
            }

            Bool AbsMethod::is_foreign_method() {
                return is_foreign;
            }

            const std::string &AbsMethod::get_signature() {
                return method_signature;
            }

            const std::string &AbsMethod::get_name() {
                return method_name;
            }
        }
    }
}