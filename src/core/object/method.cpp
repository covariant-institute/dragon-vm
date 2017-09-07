//
// Created by kiva on 2017/9/6.
//

#include <core/object/method.hpp>
#include "foreign_method.hpp"
#include "dvm_method.hpp"

namespace dvm {
    namespace core {
        namespace object {
            Method *Method::resolve(const std::string &name, const std::string &signature) {
                return nullptr;
            }

            Method *Method::new_foreign_method(const std::string &name, const std::string &signature, FFIFunction ffi) {
                return new ForeignMethod(name, signature, ffi);
            }

            Method *
            Method::new_dvm_method(const std::string &name, const std::string &signature, Byte *body, SizeT length) {
                return new DvmMethod(name, signature, body, length);
            }
        }
    }
}
