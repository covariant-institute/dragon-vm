//
// Created by kiva on 2017/8/30.
//
#pragma once

#include <core/vm_register_common.hpp>
#include <core/vm_register_visitor.hpp>
#include <core/exceptions.hpp>

namespace dvm {
    namespace core {
        namespace vm {
            namespace registers {
                class vm_context;

                class vm_register_holder {
                    friend class vm_context;

                private:
                    Link registers;

                public:
                    vm_register_holder(const vm_register_holder &) = delete;

                    vm_register_holder() throw(dvm::core::exception::exception);

                    ~vm_register_holder();

                    register_visitor get_register(vm_registers id) throw(dvm::core::exception::exception);

                    template <vm_registers vm_reg, ULong offset = sizeof(VMRegisterIX64) * static_cast<VMRegisterID>(vm_reg)>
                    register_visitor get_register() {
                        return register_visitor(registers + offset);
                    }
                };
            }
        }
    }
}