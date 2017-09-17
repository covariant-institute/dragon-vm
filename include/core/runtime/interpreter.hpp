#pragma once

#include <core/runtime/opcodes.hpp>
#include <core/runtime/vm_context.hpp>

#define OPCODE_HANDLER_NAME(X) opcode_handler_##X
#ifdef DVM_INTERPRETATION_THREADED

//#define OPCODE_HANDLER(name) void *OPCODE_HANDLER_NAME(name);
#define OPCODE_HANDLER(name)

#else

#define OPCODE_HANDLER(name) void OPCODE_HANDLER_NAME(name)();

#endif


#define OPCODE(X) OPCODE_HANDLER(X)

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
            class Interpreter {
            private:
                VMOpcode *pc;
#ifdef DVM_INTERPRETATION_THREADED
                void *opcode_jump_table[VM_OPCODES_NUMBER];
#else
                using OpcodeHandlerType = void (Interpreter::*)();
                OpcodeHandlerType opcode_jump_table[VM_OPCODES_NUMBER];
#endif

                void setup_opcode_handler();

#ifdef DVM_INTERPRETATION_THREADED

                void threaded(void **jump_table);

#endif

#include "opcodes_def.hpp.inc"

            public:
                Interpreter();

                void load_code(VMOpcode *code);

                void exec();
            };
        }
    }
}

#undef INTS_OPCODES_PREFIX
#undef FLOATS_OPCODES_PREFIX
#undef TYPED_OPCODES_PREFIX
#undef OPCODE

