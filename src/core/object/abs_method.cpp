//
// Created by kiva on 2017/9/6.
//

#include "abs_method.hpp"


namespace dvm {
    namespace core {
        namespace object {
            AbsMethod::AbsMethod(Class *return_type, const std::string &name, const std::string &signature,
                                 Bool is_native_method, Bool is_static_method)
                    : return_type(return_type), method_name(name), method_signature(signature),
                      is_native_method(is_native_method), is_static_method(is_static_method) {
            }

            Bool AbsMethod::is_native() {
                return is_native_method;
            }

            const std::string &AbsMethod::get_signature() {
                return method_signature;
            }

            const std::string &AbsMethod::get_name() {
                return method_name;
            }

            Bool AbsMethod::is_static() {
                return is_static_method;
            }

            Class *AbsMethod::get_return_type() {
                return return_type;
            }
        }
    }
}