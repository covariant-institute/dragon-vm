#pragma once

#include "core/type.hpp"

namespace dvm {
    namespace core {
        namespace runtime {
            using VMOpcode = UInt32;

#define TYPED_OPCODES(PREFIX) \
            PREFIX##_I8, \
            PREFIX##_I16, \
            PREFIX##_I32, \
            PREFIX##_I64, \
            PREFIX##_U8, \
            PREFIX##_U16, \
            PREFIX##_U32, \
            PREFIX##_U64, \
            PREFIX##_F32, \
            PREFIX##_F64, \
            PREFIX##_BYTE, \
            PREFIX##_BOOL, \
            PREFIX##_OBJECT

            /**
             * 为了方便我们使用 Threaded-Interpretation，指令一律用 UInt32
             */
            enum class VMOpcodes : VMOpcode {
                NOP = 0,

                NEW_INSTANCE,
                NEW_INSTANCE_STACK,

                INVOKE_METHOD,

                TYPED_OPCODES(PUSH),
                TYPED_OPCODES(POP),

                TYPED_OPCODES(SET_SLOT),
                TYPED_OPCODES(SET_CLASS_SLOT),
                TYPED_OPCODES(GET_SLOT),
                TYPED_OPCODES(GET_CLASS_SLOT),

                VM_OPCODES_NUMBER,
            };

            constexpr UInt32 VM_OPCODES_NUMBER = static_cast<UInt32>(VMOpcodes::VM_OPCODES_NUMBER);

#undef TYPED_OPCODES
        }
    }
}
