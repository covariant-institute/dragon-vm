//
// Created by kiva on 2017/9/17.
//

#include <core/runtime/interpreter.hpp>

#define O(X) static_cast<VMOpcode>(VMOpcodes::X)

#define OPCODE(X) O(X),

#define INTS_OPCODES_PREFIX(PREFIX) \
            OPCODE(PREFIX##_i32) \
            OPCODE(PREFIX##_i64)

#define FLOATS_OPCODES_PREFIX(PREFIX) \
            OPCODE(PREFIX##_f32) \
            OPCODE(PREFIX##_f64)

#define TYPED_OPCODES_PREFIX(PREFIX) \
            INTS_OPCODES_PREFIX(PREFIX) \
            FLOATS_OPCODES_PREFIX(PREFIX)

int main() {
    using namespace dvm::core::runtime;
    Interpreter interpreter;

    VMOpcode code[] = {
#include <core/runtime/opcodes_def.hpp.inc>
    };

    Thread thread;
    thread.set_runnable(code);
    interpreter.exec(&thread);
}
