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
                constexpr UInt32 VM_REGISTERS_NUMBER = 16;

                /**
                 * 寄存器名对应的寄存器ID
                 */
                enum class VMRegisterName : VMRegisterID {
                    R0 = 0,
                    R1,
                    R2,
                    R3,
                    R4,
                    R5,
                    R6,
                    R7,
                    R8,
                    R9,
                    R10,
                    R11,
                    R12,
                    R13,
                    R14,
                    R15,
                };
            }
        }
    }
}

