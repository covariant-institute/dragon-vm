//
// Created by kiva on 2017/8/30.
//
#pragma once

#include <core/type.hpp>
#include <core/object/object.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            namespace registers {
                /**
                 * 寄存器ID，用于标识寄存器
                 */
                using VMRegisterID = Int32;

                /**
                 * 通用对象寄存器
                 */
                using VMRegister = object::Object *;

                /**
                 * 通用对象寄存器数量
                 */
                constexpr UInt32 VM_REGISTERS_NUMBER = 8;

                /**
                 * 寄存器名对应的寄存器ID
                 */
                enum class vm_register_id : VMRegisterID {
                    VM_REG_R0 = 0,
                    VM_REG_R1,
                    VM_REG_R2,
                    VM_REG_R3,
                    VM_REG_R4,
                    VM_REG_R5,
                    VM_REG_R6,
                    VM_REG_R7,
                };
            }
        }
    }
}

