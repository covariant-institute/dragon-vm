//
// Created by kiva on 2017/9/3.
//

#include <core/runtime/vm_register_visitor.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            namespace registers {
                register_visitor::register_visitor(VMRegister *register_ptr)
                    :register_ptr(register_ptr) {
                }

                register_visitor::register_visitor(const register_visitor &other)
                    :register_ptr(other.register_ptr) {
                }
            }
        }
    }
}
