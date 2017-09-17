#pragma once

#include "core/type.hpp"

#define OPCODE(X) X,

#define INTS_OPCODES_PREFIX(PREFIX) \
            OPCODE(PREFIX##_i32) \
            OPCODE(PREFIX##_i64)

#define FLOATS_OPCODES_PREFIX(PREFIX) \
            OPCODE(PREFIX##_f32) \
            OPCODE(PREFIX##_f64)

#define TYPED_OPCODES_PREFIX(PREFIX) \
            INTS_OPCODES_PREFIX(PREFIX) \
            FLOATS_OPCODES_PREFIX(PREFIX)

namespace dvm {
    namespace core {
        namespace runtime {
            /**
             * 为了方便我们使用 Threaded-Interpretation，指令一律用 UInt8
             */
            using VMOpcode = UInt8;

            using VMOpcodeName = const char *;

            enum class VMOpcodes : VMOpcode {
#include "opcodes_def.hpp.inc"
                VM_OPCODES_NUMBER,
            };

            constexpr UInt32 VM_OPCODES_NUMBER = static_cast<UInt32>(VMOpcodes::VM_OPCODES_NUMBER);


#undef OPCODE
#define OPCODE(X) #X,

            constexpr VMOpcodeName VM_OPCODE_NAMES[VM_OPCODES_NUMBER] = {
#include "opcodes_def.hpp.inc"
            };

        }
    }
}

#undef FLOATS_OPCODES_PREFIX
#undef INTS_OPCODES_PREFIX
#undef TYPED_OPCODES_PREFIX
#undef OPCODE
