## Opcodes in Dragon VM


| Mnemonic | Opcode (in hex) | Opcode (in binary) | Other bytes ([count]: [operands]) | Stack ([before] -> [after]) | Description |
|:--------:|:---------------:|:------------------:|:---------------------------------------:|:---------------------------:|:-----------:|
| nop                    |   00   |   0000 0000   |        | [No Change] | do nothing |
| new_instance           |   01   |   0000 0001   | 2: indexbyte1, indexbyte2 | -> object | create an instance of a class, which is identified by ***#index***(`indexbyte1 << 8 + indexbyte2`) in constant pool. |
| invoke_method          |   02   |   0000 0010   | 4: nameindex1, nameindex2, sigindex1, sigindex2 | object, [arg1, arg2, ...] -> [result] | invoke method on object and puts the result on the stack (might be void); the method is identified by nameindex(`nameindex1 << 8 + nameindex2`) and sigindex(`sigindex1 << 8 + sigindex2`) in constant pool. |
| ldc_null               |   03   |   0000 0011   |        | -> null | push a ***null*** object onto the stack |
| ldc_i32                |   04   |   0000 0100   | 4: byte1, byte2, byte3, byte4 | -> value | push an ***#int32***(`byte1 << 24 + byte2 << 16 + byte3 << 8 + byte4`) onto the stack |
| ldc_i64                |   05   |   0000 0101   |        | int32h, int32l -> value | push a ***#int64***(`int32h << 32 + int32l`) |
| ldc_u32                |   06   |   0000 0110   | 4: byte1, byte2, byte3, byte4 | -> value | push a ***#uint32***(`byte1 << 24 + byte2 << 16 + byte3 << 8 + byte4`) onto the stack |
| ldc_u64                |   07   |   0000 0111   |        | uint32h, uint32l -> value | push a ***#uint64***(`uint32h << 32 + uint32l`) |
| ldc_f32                |   08   |   0000 1000   | 4: byte1, byte2, byte3, byte4 | -> value | push a ***#float***(`byte1 << 24 + byte2 << 16 + byte3 << 8 + byte4`) onto the stack |
| ldc_f64                |   09   |   0000 1001   |        | -> value | desc |
| pop                    |   0a   |   0000 1010   |        | value -> | drop the top of stack |
| ld_i32                 |   0b   |   0000 1011   | 1: index | -> value | load an int32 from register ***#index***  |
| ld_i64                 |   0c   |   0000 1100   | 1: index | -> value | load an int64 from register ***#index***  |
| ld_u32                 |   0d   |   0000 1101   | 1: index | -> value | load a uint32 from register ***#index***  |
| ld_u64                 |   0e   |   0000 1110   | 1: index | -> value | load a uint64 from register ***#index***  |
| ld_f32                 |   0f   |   0000 1111   | 1: index | -> value | load a float from register ***#index***  |
| ld_f64                 |   10   |   0001 0000   | 1: index | -> value | load a double from register ***#index***  |
| ld_object              |   11   |   0001 0001   | 1: index | -> object | load an object from register ***#index***  |
| st_i32                 |   12   |   0001 0010   | 1: index | value -> | store an int32 to register ***#index***  |
| st_i64                 |   13   |   0001 0011   | 1: index | value -> | store an int64 to register ***#index***  |
| st_u32                 |   14   |   0001 0100   | 1: index | value -> | store a uint32 to register ***#index***  |
| st_u64                 |   15   |   0001 0101   | 1: index | value -> | store a uint64 to register ***#index***  |
| st_f32                 |   16   |   0001 0110   | 1: index | value -> | store a float to register ***#index***  |
| st_f64                 |   17   |   0001 0111   | 1: index | value -> | store a double to register ***#index***  |
| st_object              |   18   |   0001 1000   | 1: index | object -> | store an object to register ***#index***  |
| set_slot_i32           |   19   |   0001 1001   |        | -> | desc |
| set_slot_i64           |   1a   |   0001 1010   |        | -> | desc |
| set_slot_u32           |   1b   |   0001 1011   |        | -> | desc |
| set_slot_u64           |   1c   |   0001 1100   |        | -> | desc |
| set_slot_f32           |   1d   |   0001 1101   |        | -> | desc |
| set_slot_f64           |   1e   |   0001 1110   |        | -> | desc |
| set_class_slot_i32     |   1f   |   0001 1111   |        | -> | desc |
| set_class_slot_i64     |   20   |   0010 0000   |        | -> | desc |
| set_class_slot_u32     |   21   |   0010 0001   |        | -> | desc |
| set_class_slot_u64     |   22   |   0010 0010   |        | -> | desc |
| set_class_slot_f32     |   23   |   0010 0011   |        | -> | desc |
| set_class_slot_f64     |   24   |   0010 0100   |        | -> | desc |
| get_slot_i32           |   25   |   0010 0101   |        | -> | desc |
| get_slot_i64           |   26   |   0010 0110   |        | -> | desc |
| get_slot_u32           |   27   |   0010 0111   |        | -> | desc |
| get_slot_u64           |   28   |   0010 1000   |        | -> | desc |
| get_slot_f32           |   29   |   0010 1001   |        | -> | desc |
| get_slot_f64           |   2a   |   0010 1010   |        | -> | desc |
| get_class_slot_i32     |   2b   |   0010 1011   |        | -> | desc |
| get_class_slot_i64     |   2c   |   0010 1100   |        | -> | desc |
| get_class_slot_u32     |   2d   |   0010 1101   |        | -> | desc |
| get_class_slot_u64     |   2e   |   0010 1110   |        | -> | desc |
| get_class_slot_f32     |   2f   |   0010 1111   |        | -> | desc |
| get_class_slot_f64     |   30   |   0011 0000   |        | -> | desc |
