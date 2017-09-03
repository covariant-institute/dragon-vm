//
// Created by kiva on 2017/8/30.
//
#pragma once

#include <core/type.hpp>

namespace dvm {
    namespace core {
        namespace interpreter {
            namespace registers {
                /**
                 * 寄存器ID，用于标识寄存器
                 */
                using VMRegisterID = Int32;

                /**
                 * 整型32位寄存器
                 */
                using VMRegisterIX32 = Int32;

                /**
                 * 整型64位寄存器
                 */
                using VMRegisterIX64 = Int64;

                /**
                 * 浮点32位寄存器
                 */
                using VMRegisterFX32 = Float;

                /**
                 * 浮点64位寄存器
                 */
                using VMRegisterFX64 = Double;

                /**
                 * 整型32位寄存器数量
                 */
                constexpr UInt32 VM_REGISTERS_IX32_NUMBER = 8;

                /**
                 * 整型64位寄存器数量
                 */
                constexpr UInt32 VM_REGISTERS_IX64_NUMBER = 4;

                /**
                 * 浮点32位寄存器数量
                 */
                constexpr UInt32 VM_REGISTERS_FX32_NUMBER = 2;

                /**
                 * 浮点64位寄存器数量
                 */
                constexpr UInt32 VM_REGISTERS_FX64_NUMBER = 2;

                constexpr UInt32 VM_REGISTERS_NUMBER = VM_REGISTERS_IX32_NUMBER + VM_REGISTERS_IX64_NUMBER
                                                     + VM_REGISTERS_FX32_NUMBER + VM_REGISTERS_FX64_NUMBER;

                /**
                 * 寄存器名对应的寄存器ID
                 */
                enum class vm_register_id : VMRegisterID {
                    /*
                     * 整型32位寄存器
                     * ix0 - ix7
                     */
                            VM_REG_IX0 = 0,
                    VM_REG_IX1,
                    VM_REG_IX2,
                    VM_REG_IX3,
                    VM_REG_IX4,
                    VM_REG_IX5,
                    VM_REG_IX6,
                    VM_REG_IX7,

                    /*
                     * 整型64位寄存器 ix8 - ix11
                     */
                            VM_REG_IX8,
                    VM_REG_IX9,
                    VM_REG_IX10,
                    VM_REG_IX11,

                    /*
                     * 浮点32位寄存器
                     * fx0 - fx1
                     */
                            VM_REG_FX0,
                    VM_REG_FX1,

                    /*
                     * 浮点64位寄存器
                     * fx2 - fx3
                     */
                            VM_REG_FX2,
                    VM_REG_FX3,
                };
            }
        }
    }
}

