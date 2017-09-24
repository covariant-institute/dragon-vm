#pragma once

#include <core/runtime/opcodes.hpp>

#define OPCODE_HANDLER_NAME(X) opcode_handler_##X
#ifdef DVM_INTERPRETATION_THREADED

//#define OPCODE_HANDLER(name) void *OPCODE_HANDLER_NAME(name);
#define OPCODE_HANDLER(name)

#else

#define OPCODE_HANDLER(name) void OPCODE_HANDLER_NAME(name)(Thread *thread);

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
            class Thread;

            class VMContext;

            class Interpreter {
                friend class Thread;

            private:
                void setup_opcode_handler();

                void exec(Thread *thread, VMContext *context);

#ifdef DVM_INTERPRETATION_THREADED
                void *opcode_jump_table[VM_OPCODES_NUMBER];

                void threaded(Thread *thread, VMContext *context, void **jump_table);
#else
                using OpcodeHandlerType = void (Interpreter::*)(Thread *, VMContext *);
                OpcodeHandlerType opcode_jump_table[VM_OPCODES_NUMBER];
#endif

#include "opcodes_def.hpp.inc"

                Interpreter();

                ~Interpreter() = default;
            };
        }
    }
}

#undef INTS_OPCODES_PREFIX
#undef FLOATS_OPCODES_PREFIX
#undef TYPED_OPCODES_PREFIX
#undef OPCODE

