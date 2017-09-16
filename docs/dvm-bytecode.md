## Opcodes in Dragon VM

### Instructions
| Mnemonic | Opcode (in hex) | Opcode (in binary) | Other bytes ([count]: [operand labels]) | Stack ([before] -> [after]) | Description |
|:--------:|:---------------:|:------------------:|:---------------------------------------:|:---------------------------:|:-----------:|
| nop                    |   00   |   0000 0000   |        | [No Change] | do nothing |
| new_instance           |   01   |   0000 0001   | 2: indexbyte1, indexbyte2 | -> object | create an instance of a class, which is identified by ***#index***(`indexbyte1 << 8 + indexbyte2`) in constant pool. |
| invoke_method          |   02   |   0000 0010   | 4: nameindex1, nameindex2, sigindex1, sigindex2 | object, [arg1, arg2, ...] -> [result] | invoke method on object and puts the result on the stack (might be void); the method is identified by nameindex(`nameindex1 << 8 + nameindex2`) and sigindex(`sigindex1 << 8 + sigindex2`) in constant pool. |
| ldc_null               |   03   |   0000 0011   |        | -> null | push a ***null*** object onto the stack |
| ldc_i32                |   04   |   0000 0100   | 4: byte1, byte2, byte3, byte4 | -> value | push an ***#int32***(`byte1 << 24 + byte2 << 16 + byte3 << 8 + byte4`) onto the stack |
| ldc_i64                |   05   |   0000 0101   |        | int32h, int32l -> value | push a ***#int64***(`int32h << 32 + int32l`) |
| ldc_f32                |   06   |   0000 0110   | 4: byte1, byte2, byte3, byte4 | -> value | push a ***#float***(`byte1 << 24 + byte2 << 16 + byte3 << 8 + byte4`) onto the stack |
| ldc_f64                |   07   |   0000 0111   |        | **TODO** -> **TODO** | **TODO** |
| pop                    |   08   |   0000 1000   |        | value -> | discard the top of stack |
| ld_i32                 |   09   |   0000 1001   | 1: index | -> value | load an int32 from register ***#index***  |
| ld_i64                 |   0a   |   0000 1010   | 1: index | -> value | load an int64 from register ***#index***  |
| ld_f32                 |   0b   |   0000 1011   | 1: index | -> value | load a float from register ***#index***  |
| ld_f64                 |   0c   |   0000 1100   | 1: index | -> value | load a double from register ***#index***  |
| ld_object              |   0d   |   0000 1101   | 1: index | -> value | load an object from register ***#index***  |
| st_i32                 |   0e   |   0000 1110   | 1: index | value -> | store an int32 to register ***#index***  |
| st_i64                 |   0f   |   0000 1111   | 1: index | value -> | store an int64 to register ***#index***  |
| st_f32                 |   10   |   0001 0000   | 1: index | value -> | store a float to register ***#index***  |
| st_f64                 |   11   |   0001 0001   | 1: index | value -> | store a double to register ***#index***  |
| st_object              |   12   |   0001 0010   | 1: index | value -> | store an object to register ***#index***  |
| add_i32                |   13   |   0001 0011   |        | value1, value2 -> result | add two int32s |
| add_i64                |   14   |   0001 0100   |        | value1, value2 -> result | add two int64s |
| add_f32                |   15   |   0001 0101   |        | value1, value2 -> result | add two floats |
| add_f64                |   16   |   0001 0110   |        | value1, value2 -> result | add two doubles |
| sub_i32                |   17   |   0001 0111   |        | value1, value2 -> result | subtract two int32s |
| sub_i64                |   18   |   0001 1000   |        | value1, value2 -> result | subtract two int64s |
| sub_f32                |   19   |   0001 1001   |        | value1, value2 -> result | subtract two floats |
| sub_f64                |   1a   |   0001 1010   |        | value1, value2 -> result | subtract two doubles |
| mul_i32                |   1b   |   0001 1011   |        | value1, value2 -> result | multiply two int32s |
| mul_i64                |   1c   |   0001 1100   |        | value1, value2 -> result | multiply two int64s |
| mul_f32                |   1d   |   0001 1101   |        | value1, value2 -> result | multiply two floats |
| mul_f64                |   1e   |   0001 1110   |        | value1, value2 -> result | multiply two doubles |
| div_i32                |   1f   |   0001 1111   |        | value1, value2 -> result | divide two int32s |
| div_i64                |   20   |   0010 0000   |        | value1, value2 -> result | divide two int64s |
| div_f32                |   21   |   0010 0001   |        | value1, value2 -> result | divide two floats |
| div_f64                |   22   |   0010 0010   |        | value1, value2 -> result | divide two doubles |
| remain_i32             |   23   |   0010 0011   |        | value1, value2 -> result | get the remainder from a division between two int32s`(value1 % value2)` |
| remain_i64             |   24   |   0010 0100   |        | value1, value2 -> result | get the remainder from a division between two int64s`(value1 % value2)` |
| remain_f32             |   25   |   0010 0101   |        | value1, value2 -> result | get the remainder from a division between two two floats`(value1 % value2)` |
| remain_f64             |   26   |   0010 0110   |        | value1, value2 -> result | get the remainder from a division between two doubles`(value1 % value2)` |
| neg_i32                |   27   |   0010 0111   |        | value -> result | negate an int32 |
| neg_i64                |   28   |   0010 1000   |        | value -> result | negate an int64 |
| neg_f32                |   29   |   0010 1001   |        | value -> result | negate an float |
| neg_f64                |   2a   |   0010 1010   |        | value -> result | negate an double |
| shl_i32                |   2b   |   0010 1011   |        | value1, value2 -> result | bitwise shift left of a int32 value1 by int32 value2 positions |
| shl_i64                |   2c   |   0010 1100   |        | value1, value2 -> result | bitwise shift left of a int64 value1 by int32 value2 positions |
| shr_i32                |   2d   |   0010 1101   |        | value1, value2 -> result | bitwise shift right of a int32 value1 by int32 value2 positions |
| shr_i64                |   2e   |   0010 1110   |        | value1, value2 -> result | bitwise shift right of a int64 value1 by int32 value2 positions |
| ushl_i32               |   2f   |   0010 1111   |        | value1, value2 -> result | bitwise shift left of a int32 value1 by int32 value2 positions, unsigned |
| ushl_i64               |   30   |   0011 0000   |        | value1, value2 -> result | bitwise shift left of a int64 value1 by int32 value2 positions, unsigned |
| ushr_i32               |   31   |   0011 0001   |        | value1, value2 -> result | bitwise shift right of a int32 value1 by int32 value2 positions, unsigned |
| ushr_i64               |   32   |   0011 0010   |        | value1, value2 -> result | bitwise shift right of a int64 value1 by int32 value2 positions, unsigned |
| and_i32                |   33   |   0011 0011   |        | value1, value2 -> result | bitwise AND of two int32s |
| and_i64                |   34   |   0011 0100   |        | value1, value2 -> result | bitwise AND of two int64s |
| or_i32                 |   35   |   0011 0101   |        | value1, value2 -> result | bitwise OR of two int32s |
| or_i64                 |   36   |   0011 0110   |        | value1, value2 -> result | bitwise OR of two int64s |
| xor_i32                |   37   |   0011 0111   |        | value1, value2 -> result | bitwise XOR of two int32s |
| xor_i64                |   38   |   0011 1000   |        | value1, value2 -> result | bitwise XOR of two int64s |
| i32_to_i32             |   39   |   0011 1001   |        | value -> result | useless, should not appear in any class file |
| i32_to_i64             |   3a   |   0011 1010   |        | value -> result | convert an int32 to int64 |
| i32_to_f32             |   3b   |   0011 1011   |        | value -> result | convert an int32 to float |
| i32_to_f64             |   3c   |   0011 1100   |        | value -> result | convert an int32 to double |
| i64_to_i32             |   3d   |   0011 1101   |        | value -> result | convert an int64 to int32 |
| i64_to_i64             |   3e   |   0011 1110   |        | value -> result | useless, should not appear in any class file |
| i64_to_f32             |   3f   |   0011 1111   |        | value -> result | convert an int64 to float |
| i64_to_f64             |   40   |   0100 0000   |        | value -> result | convert an int64 to double |
| f32_to_i32             |   41   |   0100 0001   |        | value -> result | convert a float to int32 |
| f32_to_i64             |   42   |   0100 0010   |        | value -> result | convert a float to int64 |
| f32_to_f32             |   43   |   0100 0011   |        | value -> result | useless, should not appear in any class file |
| f32_to_f64             |   44   |   0100 0100   |        | value -> result | convert a float to double |
| f64_to_i32             |   45   |   0100 0101   |        | value -> result | convert a double to int32 |
| f64_to_i64             |   46   |   0100 0110   |        | value -> result | convert a double to int64 |
| f64_to_f32             |   47   |   0100 0111   |        | value -> result | convert a double to float |
| f64_to_f64             |   48   |   0100 1000   |        | value -> result | useless, should not appear in any class file |
