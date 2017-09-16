OPCODE(nop),
OPCODE(new_instance),
OPCODE(invoke_method),

/* ldc: constant to stack */
OPCODE(ldc_null),
TYPED_OPCODES_PREFIX(ldc),

/* pop: drop stack top */
OPCODE(pop),

/* ld: register to stack */
TYPED_OPCODES_PREFIX(ld),
OPCODE(ld_object),

/* st: stack to register */
TYPED_OPCODES_PREFIX(st),
OPCODE(st_object),

TYPED_OPCODES_PREFIX(set_slot),
TYPED_OPCODES_PREFIX(set_class_slot),
TYPED_OPCODES_PREFIX(get_slot),
TYPED_OPCODES_PREFIX(get_class_slot),
