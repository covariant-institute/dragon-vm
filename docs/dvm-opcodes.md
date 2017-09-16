## Opcodes in Dragon VM

| Mnemonic | Opcode (in hex) | Opcode (in binary) | Other bytes ([count]: [operand labels]) | Stack ([before] -> [after]) | Description |
|:--------:|:---------------:|:------------------:|:---------------------------------------:|:---------------------------:|:-----------:|
| nop                    |   00   |   0000 0000   |        | [No Change] | do nothing |
| new_instance           |   01   |   0000 0001   | 2: indexbyte1, indexbyte2   | -> object | Create an instance of a class, which is identified by *#index*(`indexbyte1 << 8 + indexbyte2`)|
| invoke_method          |   02   |   0000 0010   |        | -> | desc |
| push_null              |   03   |   0000 0011   |        | -> | desc |
| push_i32               |   04   |   0000 0100   |        | -> | desc |
| push_i64               |   05   |   0000 0101   |        | -> | desc |
| push_u32               |   06   |   0000 0110   |        | -> | desc |
| push_u64               |   07   |   0000 0111   |        | -> | desc |
| push_f32               |   08   |   0000 1000   |        | -> | desc |
| push_f64               |   09   |   0000 1001   |        | -> | desc |
| push_object            |   0a   |   0000 1010   |        | -> | desc |
| pop_i32                |   0b   |   0000 1011   |        | -> | desc |
| pop_i64                |   0c   |   0000 1100   |        | -> | desc |
| pop_u32                |   0d   |   0000 1101   |        | -> | desc |
| pop_u64                |   0e   |   0000 1110   |        | -> | desc |
| pop_f32                |   0f   |   0000 1111   |        | -> | desc |
| pop_f64                |   10   |   0001 0000   |        | -> | desc |
| pop_object             |   11   |   0001 0001   |        | -> | desc |
| set_slot_i32           |   12   |   0001 0010   |        | -> | desc |
| set_slot_i64           |   13   |   0001 0011   |        | -> | desc |
| set_slot_u32           |   14   |   0001 0100   |        | -> | desc |
| set_slot_u64           |   15   |   0001 0101   |        | -> | desc |
| set_slot_f32           |   16   |   0001 0110   |        | -> | desc |
| set_slot_f64           |   17   |   0001 0111   |        | -> | desc |
| set_slot_object        |   18   |   0001 1000   |        | -> | desc |
| set_class_slot_i32     |   19   |   0001 1001   |        | -> | desc |
| set_class_slot_i64     |   1a   |   0001 1010   |        | -> | desc |
| set_class_slot_u32     |   1b   |   0001 1011   |        | -> | desc |
| set_class_slot_u64     |   1c   |   0001 1100   |        | -> | desc |
| set_class_slot_f32     |   1d   |   0001 1101   |        | -> | desc |
| set_class_slot_f64     |   1e   |   0001 1110   |        | -> | desc |
| set_class_slot_object  |   1f   |   0001 1111   |        | -> | desc |
| get_slot_i32           |   20   |   0010 0000   |        | -> | desc |
| get_slot_i64           |   21   |   0010 0001   |        | -> | desc |
| get_slot_u32           |   22   |   0010 0010   |        | -> | desc |
| get_slot_u64           |   23   |   0010 0011   |        | -> | desc |
| get_slot_f32           |   24   |   0010 0100   |        | -> | desc |
| get_slot_f64           |   25   |   0010 0101   |        | -> | desc |
| get_slot_object        |   26   |   0010 0110   |        | -> | desc |
| get_class_slot_i32     |   27   |   0010 0111   |        | -> | desc |
| get_class_slot_i64     |   28   |   0010 1000   |        | -> | desc |
| get_class_slot_u32     |   29   |   0010 1001   |        | -> | desc |
| get_class_slot_u64     |   2a   |   0010 1010   |        | -> | desc |
| get_class_slot_f32     |   2b   |   0010 1011   |        | -> | desc |
| get_class_slot_f64     |   2c   |   0010 1100   |        | -> | desc |
| get_class_slot_object  |   2d   |   0010 1101   |        | -> | desc |
