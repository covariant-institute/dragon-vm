## Opcodes in Dragon VM

### Instructions
| Mnemonic | Opcode (in hex) | Opcode (in binary) | Other bytes ([count]: [operand labels]) | Stack ([before] → [after]) ([top], ..., [bottom]) | Description |
|:--------:|:---------------:|:------------------:|:---------------------------------------:|:---------------------------:|:-----------:|
| nop                    |   00   |   0000 0000   |        | [No Change] | do nothing |
| new_instance           |   01   |   0000 0001   | 2: indexbyte1, indexbyte2 | → object | create an instance of a class identified by ***#index***(`indexbyte1 << 8 + indexbyte2`) in constant pool. |
| invoke_method          |   02   |   0000 0010   | 4: nameindex1, nameindex2, sigindex1, sigindex2 | object, [arg1, arg2, ...] → [result] | invoke method on object and puts the result on the stack (might be void); the method is identified by nameindex(`nameindex1 << 8 + nameindex2`) and sigindex(`sigindex1 << 8 + sigindex2`) in constant pool. |
| ldc_null               |   03   |   0000 0011   |        | → null | push a ***null*** object onto the stack |
| ldc_i32                |   04   |   0000 0100   | 4: byte1, byte2, byte3, byte4 | → value | push an ***#int32***(`byte1 << 24 + byte2 << 16 + byte3 << 8 + byte4`) onto the stack |
| ldc_i64                |   05   |   0000 0101   |        | int32h, int32l → value | push a ***#int64***(`int32h << 32 + int32l`) |
| ldc_f32                |   06   |   0000 0110   | 4: byte1, byte2, byte3, byte4 | → value | push a ***#float***(`byte1 << 24 + byte2 << 16 + byte3 << 8 + byte4`) onto the stack |
| ldc_f64                |   07   |   0000 0111   |        | **TODO** → **TODO** | **TODO** |
| sys                    |   08   |   0000 1000   |        | **TODO** → **TODO** | **TODO** |
| ld_i32                 |   09   |   0000 1001   | 1: index | → value | load an int32 from register ***#index***  |
| ld_i64                 |   0a   |   0000 1010   | 1: index | → value | load an int64 from register ***#index***  |
| ld_f32                 |   0b   |   0000 1011   | 1: index | → value | load a float from register ***#index***  |
| ld_f64                 |   0c   |   0000 1100   | 1: index | → value | load a double from register ***#index***  |
| ld_object              |   0d   |   0000 1101   | 1: index | → value | load an object from register ***#index***  |
| st_i32                 |   0e   |   0000 1110   | 1: index | [No Change] | store an int32 to register ***#index***  |
| st_i64                 |   0f   |   0000 1111   | 1: index | [No Change] | store an int64 to register ***#index***  |
| st_f32                 |   10   |   0001 0000   | 1: index | [No Change] | store a float to register ***#index***  |
| st_f64                 |   11   |   0001 0001   | 1: index | [No Change] | store a double to register ***#index***  |
| st_object              |   12   |   0001 0010   | 1: index | [No Change] | store an object to register ***#index***  |
| add_i32                |   13   |   0001 0011   |        | value1, value2 → result | add two int32s |
| add_i64                |   14   |   0001 0100   |        | value1, value2 → result | add two int64s |
| add_f32                |   15   |   0001 0101   |        | value1, value2 → result | add two floats |
| add_f64                |   16   |   0001 0110   |        | value1, value2 → result | add two doubles |
| sub_i32                |   17   |   0001 0111   |        | value1, value2 → result | subtract two int32s |
| sub_i64                |   18   |   0001 1000   |        | value1, value2 → result | subtract two int64s |
| sub_f32                |   19   |   0001 1001   |        | value1, value2 → result | subtract two floats |
| sub_f64                |   1a   |   0001 1010   |        | value1, value2 → result | subtract two doubles |
| mul_i32                |   1b   |   0001 1011   |        | value1, value2 → result | multiply two int32s |
| mul_i64                |   1c   |   0001 1100   |        | value1, value2 → result | multiply two int64s |
| mul_f32                |   1d   |   0001 1101   |        | value1, value2 → result | multiply two floats |
| mul_f64                |   1e   |   0001 1110   |        | value1, value2 → result | multiply two doubles |
| div_i32                |   1f   |   0001 1111   |        | value1, value2 → result | divide two int32s |
| div_i64                |   20   |   0010 0000   |        | value1, value2 → result | divide two int64s |
| div_f32                |   21   |   0010 0001   |        | value1, value2 → result | divide two floats |
| div_f64                |   22   |   0010 0010   |        | value1, value2 → result | divide two doubles |
| remain_i32             |   23   |   0010 0011   |        | value1, value2 → result | get the remainder from a division between two int32s`(value1 % value2)` |
| remain_i64             |   24   |   0010 0100   |        | value1, value2 → result | get the remainder from a division between two int64s`(value1 % value2)` |
| remain_f32             |   25   |   0010 0101   |        | value1, value2 → result | get the remainder from a division between two two floats`(value1 % value2)` |
| remain_f64             |   26   |   0010 0110   |        | value1, value2 → result | get the remainder from a division between two doubles`(value1 % value2)` |
| neg_i32                |   27   |   0010 0111   |        | value → result | negate an int32 |
| neg_i64                |   28   |   0010 1000   |        | value → result | negate an int64 |
| neg_f32                |   29   |   0010 1001   |        | value → result | negate an float |
| neg_f64                |   2a   |   0010 1010   |        | value → result | negate an double |
| shl_i32                |   2b   |   0010 1011   |        | value1, value2 → result | bitwise shift left of a int32 value1 by int32 value2 positions |
| shl_i64                |   2c   |   0010 1100   |        | value1, value2 → result | bitwise shift left of a int64 value1 by int32 value2 positions |
| shr_i32                |   2d   |   0010 1101   |        | value1, value2 → result | bitwise shift right of a int32 value1 by int32 value2 positions |
| shr_i64                |   2e   |   0010 1110   |        | value1, value2 → result | bitwise shift right of a int64 value1 by int32 value2 positions |
| ushl_i32               |   2f   |   0010 1111   |        | value1, value2 → result | bitwise shift left of a int32 value1 by int32 value2 positions, unsigned |
| ushl_i64               |   30   |   0011 0000   |        | value1, value2 → result | bitwise shift left of a int64 value1 by int32 value2 positions, unsigned |
| ushr_i32               |   31   |   0011 0001   |        | value1, value2 → result | bitwise shift right of a int32 value1 by int32 value2 positions, unsigned |
| ushr_i64               |   32   |   0011 0010   |        | value1, value2 → result | bitwise shift right of a int64 value1 by int32 value2 positions, unsigned |
| and_i32                |   33   |   0011 0011   |        | value1, value2 → result | bitwise AND of two int32s |
| and_i64                |   34   |   0011 0100   |        | value1, value2 → result | bitwise AND of two int64s |
| or_i32                 |   35   |   0011 0101   |        | value1, value2 → result | bitwise OR of two int32s |
| or_i64                 |   36   |   0011 0110   |        | value1, value2 → result | bitwise OR of two int64s |
| xor_i32                |   37   |   0011 0111   |        | value1, value2 → result | bitwise XOR of two int32s |
| xor_i64                |   38   |   0011 1000   |        | value1, value2 → result | bitwise XOR of two int64s |
| i32_to_i32             |   39   |   0011 1001   |        | value → result | useless, should not appear in any class file |
| i32_to_i64             |   3a   |   0011 1010   |        | value → result | convert an int32 to int64 |
| i32_to_f32             |   3b   |   0011 1011   |        | value → result | convert an int32 to float |
| i32_to_f64             |   3c   |   0011 1100   |        | value → result | convert an int32 to double |
| i64_to_i32             |   3d   |   0011 1101   |        | value → result | convert an int64 to int32 |
| i64_to_i64             |   3e   |   0011 1110   |        | value → result | useless, should not appear in any class file |
| i64_to_f32             |   3f   |   0011 1111   |        | value → result | convert an int64 to float |
| i64_to_f64             |   40   |   0100 0000   |        | value → result | convert an int64 to double |
| f32_to_i32             |   41   |   0100 0001   |        | value → result | convert a float to int32 |
| f32_to_i64             |   42   |   0100 0010   |        | value → result | convert a float to int64 |
| f32_to_f32             |   43   |   0100 0011   |        | value → result | useless, should not appear in any class file |
| f32_to_f64             |   44   |   0100 0100   |        | value → result | convert a float to double |
| f64_to_i32             |   45   |   0100 0101   |        | value → result | convert a double to int32 |
| f64_to_i64             |   46   |   0100 0110   |        | value → result | convert a double to int64 |
| f64_to_f32             |   47   |   0100 0111   |        | value → result | convert a double to float |
| f64_to_f64             |   48   |   0100 1000   |        | value → result | useless, should not appear in any class file |
| ret                    |   49   |   0100 1001   |        | → [Empty] | return void |
| ret_object             |   4a   |   0100 1010   |        | object → [Empty] | return an object |
| ret_i32                |   4b   |   0100 1011   |        | value → [Empty] | return an int32 |
| ret_i64                |   4c   |   0100 1100   |        | value → [Empty] | return an int64 |
| ret_f32                |   4d   |   0100 1101   |        | value → [Empty] | return a float |
| ret_f64                |   4e   |   0100 1110   |        | value → [Empty] | return a double |
| jump                   |   4f   |   0100 1111   | 2: offsetbyte1, offsetbyte2 | [No Change] | goes to another instruction at `pc + offset` (signed short constructed from unsigned bytes `offsetbyte1 << 8 + offsetbyte2`) |
| jump_w                 |   50   |   0101 0000   | 4: offsetbyte1, offsetbyte2, offsetbyte3, offsetbyte4 | [No Change] | goes to another instruction at `pc + offset` (signed int constructed from unsigned bytes `offsetbyte1 << 24 + offsetbyte2 << 16 + offsetbyte3 << 8 + offsetbyte4`) |
| jump_eq                |   51   |   0101 0001   | 2: offsetbyte1, offsetbyte2 | value → | if value == 0, jump to instruction at `pc + offset` (signed short constructed from unsigned bytes `offsetbyte1 << 8 + offsetbyte2`) |
| jump_ne                |   52   |   0101 0010   | 2: offsetbyte1, offsetbyte2 | value → | if value != 0, jump to instruction at `pc + offset` (signed short constructed from unsigned bytes `offsetbyte1 << 8 + offsetbyte2`) |
| jump_gt                |   53   |   0101 0011   | 2: offsetbyte1, offsetbyte2 | value → | if value > 0, jump to instruction at `pc + offset` (signed short constructed from unsigned bytes `offsetbyte1 << 8 + offsetbyte2`) |
| jump_ge                |   54   |   0101 0100   | 2: offsetbyte1, offsetbyte2 | value → | if value >= 0, jump to instruction at `pc + offset` (signed short constructed from unsigned bytes `offsetbyte1 << 8 + offsetbyte2`) |
| jump_lt                |   55   |   0101 0101   | 2: offsetbyte1, offsetbyte2 | value → | if value < 0, jump to instruction at `pc + offset` (signed short constructed from unsigned bytes `offsetbyte1 << 8 + offsetbyte2`) |
| jump_le                |   56   |   0101 0110   | 2: offsetbyte1, offsetbyte2 | value → | if value <= 0, jump to instruction at `pc + offset` (signed short constructed from unsigned bytes `offsetbyte1 << 8 + offsetbyte2`) |
| cmp_i32                |   57   |   0101 0111   |        | value1, value2 → result | compare two int32s, push 0 if the two int32s are the same, 1 if value1 > value2, -1 otherwise |
| cmp_i64                |   58   |   0101 1000   |        | value1, value2 → result | compare two int64s, push 0 if the two int64s are the same, 1 if value1 > value2, -1 otherwise |
| cmp_lt_f32             |   59   |   0101 1001   |        | value1, value2 → result | compare two floats, push 0 if value1 < value2, 1 otherwise |
| cmp_lt_f64             |   5a   |   0101 1010   |        | value1, value2 → result | compare two doubles, push 0 if value1 < value2, 1 otherwise |
| cmp_gt_f32             |   5b   |   0101 1011   |        | value1, value2 → result | compare two floats, push 0 if value1 > value2, 1 otherwise |
| cmp_gt_f64             |   5c   |   0101 1100   |        | value1, value2 → result | compare two doubles, push 0 if value1 > value2, 1 otherwise |
| cmp_object             |   5d   |   0101 1101   |        | object1, object2 → result | compare two objects, push 0 if object1 and object2 point to the same object, 1 otherwise |
| cmp_nn_object          |   5e   |   0101 1110   |        | object1 → result | push 0 if object1 is not null, 1 otherwise |
| stp_i32                |   5f   |   0101 1111   | 3: offsetbyte1, offsetbyte2, index | [No Change] | store an int32 from stack `bp - offset`(signed short constructed from unsigned bytes `offsetbyte1 << 8 + offsetbyte2`)  to register ***#index*** |
| stp_i64                |   60   |   0110 0000   | 3: offsetbyte1, offsetbyte2, index | [No Change] | store an int64 from stack `bp - offset`(signed short constructed from unsigned bytes `offsetbyte1 << 8 + offsetbyte2`)  to register ***#index*** |
| stp_f32                |   61   |   0110 0001   | 3: offsetbyte1, offsetbyte2, index | [No Change] | store a float from stack `bp - offset`(signed short constructed from unsigned bytes `offsetbyte1 << 8 + offsetbyte2`)  to register ***#index*** |
| stp_f64                |   62   |   0110 0010   | 3: offsetbyte1, offsetbyte2, index | [No Change] | store a double from stack `bp - offset`(signed short constructed from unsigned bytes `offsetbyte1 << 8 + offsetbyte2`)  to register ***#index*** |
| stp_object             |   63   |   0110 0011   | 3: offsetbyte1, offsetbyte2, index | [No Change] | store an object from stack `bp - offset`(signed short constructed from unsigned bytes `offsetbyte1 << 8 + offsetbyte2`)  to register ***#index*** |
| pop_i32                |   64   |   0110 0100   |        | value → | discard the top of stack |
| pop_i64                |   65   |   0110 0101   |        | value → | discard the top of stack |
| pop_f32                |   66   |   0110 0110   |        | value → | discard the top of stack |
| pop_f64                |   67   |   0110 0111   |        | value → | discard the top of stack |
| pop_object             |   68   |   0110 1000   |        | object → | discard the top of stack |
| thr                    |   69   |   0110 1001   |        | object → | throw an exception object |
| halt                   |   6a   |   0110 1010   |        | [No Change] | halt VM |
| set_slot_i32           |   6b   |   0110 1011   | 1: index | object, value → | store an int32 to object slot ***#index*** |
| set_slot_i64           |   6c   |   0110 1100   | 1: index | object, value → | store an int64 to object slot ***#index*** |
| set_slot_f32           |   6d   |   0110 1101   | 1: index | object, value → | store a float to object slot ***#index*** |
| set_slot_f64           |   6e   |   0110 1110   | 1: index | object, value → | store a double to object slot ***#index*** |
| set_slot_object        |   6f   |   0110 1111   | 1: index | object, object → | store an object to object slot ***#index*** |
| set_class_slot_i32     |   70   |   0111 0000   | 3: indexbyte1, indexbyte2, slotindex | value → | store an int32 to class identified by ***#index***(`indexbyte1 << 8 + indexbyte2`) slot ***#index*** |
| set_class_slot_i64     |   71   |   0111 0001   | 3: indexbyte1, indexbyte2, slotindex | value → | store an int64 to class identified by ***#index***(`indexbyte1 << 8 + indexbyte2`) slot ***#index*** |
| set_class_slot_f32     |   72   |   0111 0010   | 3: indexbyte1, indexbyte2, slotindex | value → | store a float to class identified by ***#index***(`indexbyte1 << 8 + indexbyte2`) slot ***#index*** |
| set_class_slot_f64     |   73   |   0111 0011   | 3: indexbyte1, indexbyte2, slotindex | value → | store a double to class identified by ***#index***(`indexbyte1 << 8 + indexbyte2`) slot ***#index*** |
| set_class_slot_object  |   74   |   0111 0100   | 3: indexbyte1, indexbyte2, slotindex | object → | store an object to class identified by ***#index***(`indexbyte1 << 8 + indexbyte2`) slot ***#index*** |
| get_slot_i32           |   75   |   0111 0101   | 1: index | object → value | load an int32 from object slot ***#index*** |
| get_slot_i64           |   76   |   0111 0110   | 1: index | object → value | load an int64 from object slot ***#index*** |
| get_slot_f32           |   77   |   0111 0111   | 1: index | object → value | load a float from object slot ***#index*** |
| get_slot_f64           |   78   |   0111 1000   | 1: index | object → value | load a double from object slot ***#index*** |
| get_slot_object        |   79   |   0111 1001   | 1: index | object → object | load an object from object slot ***#index*** |
| get_class_slot_i32     |   7a   |   0111 1010   | 3: indexbyte1, indexbyte2, slotindex | → value | load an int32 from class identified by ***#index***(`indexbyte1 << 8 + indexbyte2`) slot ***#index*** |
| get_class_slot_i64     |   7b   |   0111 1011   | 3: indexbyte1, indexbyte2, slotindex | → value | load an int64 from class identified by ***#index***(`indexbyte1 << 8 + indexbyte2`) slot ***#index*** |
| get_class_slot_f32     |   7c   |   0111 1100   | 3: indexbyte1, indexbyte2, slotindex | → value | load a float from class identified by ***#index***(`indexbyte1 << 8 + indexbyte2`) slot ***#index*** |
| get_class_slot_f64     |   7d   |   0111 1101   | 3: indexbyte1, indexbyte2, slotindex | → value | load a double from class identified by ***#index***(`indexbyte1 << 8 + indexbyte2`) slot ***#index*** |
| get_class_slot_object  |   7e   |   0111 1110   | 3: indexbyte1, indexbyte2, slotindex | → object | load an object from class identified by ***#index***(`indexbyte1 << 8 + indexbyte2`) slot ***#index*** |
