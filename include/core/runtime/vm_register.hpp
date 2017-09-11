//
// Created by kiva on 2017/8/30.
//
#pragma once

#include <core/runtime/vm_register_common.hpp>
#include <core/runtime/vm_register_visitor.hpp>
#include <core/exceptions.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            namespace registers {
                class vm_context;

                class vm_register_holder {
                    friend class vm_context;

                private:
                    VMRegister *registers;

                public:
                    vm_register_holder(const vm_register_holder &) = delete;

                    vm_register_holder();

                    ~vm_register_holder();

                    register_visitor get_register(vm_register_id id);

                    template <vm_register_id vm_reg, Int32 index = static_cast<VMRegisterID>(vm_reg)>
                    register_visitor get_register() {
                        return register_visitor(registers + index);
                    }
                };
            }
        }
    }
}
