OPCODE(nop)
OPCODE(new_instance)
OPCODE(invoke_method)

/* ldc: constant to stack */
OPCODE(ldc_null)
TYPED_OPCODES_PREFIX(ldc)

/* sys: system call */
OPCODE(sys)

/* ld: register to stack */
TYPED_OPCODES_PREFIX(ld)
OPCODE(ld_ref)

/* st: stack to register */
TYPED_OPCODES_PREFIX(st)
OPCODE(st_ref)

TYPED_OPCODES_PREFIX(add)
TYPED_OPCODES_PREFIX(sub)
TYPED_OPCODES_PREFIX(mul)
TYPED_OPCODES_PREFIX(div)
TYPED_OPCODES_PREFIX(remain)
TYPED_OPCODES_PREFIX(neg)

INTS_OPCODES_PREFIX(shl)
INTS_OPCODES_PREFIX(shr)
INTS_OPCODES_PREFIX(ushl)
INTS_OPCODES_PREFIX(ushr)
INTS_OPCODES_PREFIX(and)
INTS_OPCODES_PREFIX(or)
INTS_OPCODES_PREFIX(xor)

TYPED_OPCODES_PREFIX(i32_to)
TYPED_OPCODES_PREFIX(i64_to)
TYPED_OPCODES_PREFIX(f32_to)
TYPED_OPCODES_PREFIX(f64_to)

OPCODE(ret)
OPCODE(ret_ref)
TYPED_OPCODES_PREFIX(ret)

OPCODE(jump)
OPCODE(jump_w)
OPCODE(jump_eq)
OPCODE(jump_ne)
OPCODE(jump_gt)
OPCODE(jump_ge)
OPCODE(jump_lt)
OPCODE(jump_le)

INTS_OPCODES_PREFIX(cmp)
FLOATS_OPCODES_PREFIX(cmp_lt)
FLOATS_OPCODES_PREFIX(cmp_gt)
OPCODE(cmp_ref)
OPCODE(cmp_nn_ref)

/* stp: any stack to register */
TYPED_OPCODES_PREFIX(stp)
OPCODE(stp_ref)

/* pop: drop the top of stack */
TYPED_OPCODES_PREFIX(pop)
OPCODE(pop_ref)

OPCODE(thr)
OPCODE(halt)

/* oop instructions */
TYPED_OPCODES_PREFIX(set_slot)
OPCODE(set_slot_ref)

TYPED_OPCODES_PREFIX(set_class_slot)
OPCODE(set_class_slot_ref)

TYPED_OPCODES_PREFIX(get_slot)
OPCODE(get_slot_ref)

TYPED_OPCODES_PREFIX(get_class_slot)
OPCODE(get_class_slot_ref)
