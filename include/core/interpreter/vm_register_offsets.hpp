//
// Created by kiva on 2017/8/30.
//
#pragma once

#include <core/interpreter/vm_register_common.hpp>
#include <cstddef>

namespace dvm {
    namespace core {
        namespace interpreter {
            namespace registers {
                namespace offsets {
                    constexpr size_t REGISTER_OFFSET_TABLE[] = {
                            sizeof(VMRegisterIX64) * 0,
                            sizeof(VMRegisterIX64) * 1,
                            sizeof(VMRegisterIX64) * 2,
                            sizeof(VMRegisterIX64) * 3,
                            sizeof(VMRegisterIX64) * 4,
                            sizeof(VMRegisterIX64) * 5,
                            sizeof(VMRegisterIX64) * 6,
                            sizeof(VMRegisterIX64) * 7,
                            sizeof(VMRegisterIX64) * 8,
                            sizeof(VMRegisterIX64) * 9,
                            sizeof(VMRegisterIX64) * 10,
                            sizeof(VMRegisterIX64) * 11,
                            sizeof(VMRegisterIX64) * 12,
                            sizeof(VMRegisterIX64) * 13,
                            sizeof(VMRegisterIX64) * 14,
                            sizeof(VMRegisterIX64) * 15,
                    };
                }
            }
        }
    }
}
