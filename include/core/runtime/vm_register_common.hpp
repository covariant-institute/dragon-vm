//
// Created by kiva on 2017/8/30.
//
#pragma once

#include <core/type.hpp>
#include <core/object/object.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            /**
             * 寄存器ID，用于标识寄存器
             */
            using VMRegisterID = UInt8;

            /**
             * 通用对象寄存器
             */
            using VMRegister = object::Slot;

            /**
             * 通用对象寄存器数量
             */
            constexpr UInt32 VM_REGISTERS_NUMBER = 16;
        }
    }
}

