//
// Created by kiva on 2017/9/3.
//

#include <core/runtime/vm_register_visitor.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            namespace registers {
                RegisterVisitor::RegisterVisitor(VMRegister *register_ptr)
                    :register_ptr(register_ptr) {
                }

                RegisterVisitor::RegisterVisitor(const RegisterVisitor &other)
                    :register_ptr(other.register_ptr) {
                }
            }
        }
    }
}

