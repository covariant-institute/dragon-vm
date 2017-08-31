//
// Created by kiva on 2017/8/30.
//

#include <core/interpreter/vm_register_visitor.hpp>


namespace dvm {
    namespace core {
        namespace interpreter {
            namespace registers {
                register_visitor::register_visitor(const register_visitor &other) {
                    this->register_addr = other.register_addr;
                }

                register_visitor::register_visitor(Link addr)
                : register_addr(addr) { }
            }
        }
    }
}

