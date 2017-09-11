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
            class VMContext;

            namespace registers {
                class VMRegisterHolder {
                    friend class VMContext;

                private:
                    VMRegister *registers;

                public:
                    VMRegisterHolder(const VMRegisterHolder &) = delete;

                    VMRegisterHolder();

                    ~VMRegisterHolder();

                    RegisterVisitor get_register(VMRegisterName id);

                    template <VMRegisterName vm_reg, Int32 index = static_cast<VMRegisterID>(vm_reg)>
                    RegisterVisitor get_register() {
                        return RegisterVisitor(registers + index);
                    }
                };
            }
        }
    }
}
