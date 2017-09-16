## Dragon VM Executable Binary format

### Overview
DVM has its custom executable binary format, which is called `.dcx`

Each `.dcx` file contains the following information(ordered):
* DCX Magic
* Dragon VM Version ID
* Pool Headers
* Pools

### DCX Magic
An unique magic code to ensure that this file can be loaded into DVM.

Currently, this magic code is `0xDC`. `D` stands for `DVM` and `C` stands for `Covariant`.

### Dragon VM Version ID
We cannot run a DCX file which is incompatible with the current VM version,
so we have to check its requirements(also called `VersionID`) before load it into memory.

Usually, VersionID is a combination of a magic code and three versions: `major`, `minor` and `patch`.

Talk is cheap, now I will show you how we do this:
```C++
VersionId = (magic code << 24) 
            | (major version << 16) 
            | (minor version << 8) 
            | (patch version);
```
As you see, to simplify the validation process, **we check magic code and VersionID together**.

### Pool Headers
Three headers now:
```c++
DcxFileConstantPoolHeader constant_pool_header;
DcxFileClassPoolHeader class_pool_header;
DcxFileMethodPoolHeader method_pool_header;
```

Each header structure contains a `UInt32` field named `*_entries`, which is set to the element count in the pool.

### Pools
DCX file uses pools to store information required by program.
Each pool element has a `header`, which contains its data whose type is primitive such as `Int`, `Double` etc., 
and a `container`, which contains data like `Byte *`.

#### Constant Pool
Exactly speaking, `constant pool` is `string constant pool`, only string constants are stored in this pool.

Pool element:
```c++
struct DcxFileConstantEntryHeader {
    UInt16 constant_id;
    UInt16 constant_type;
    UInt32 constant_data_size;
};
struct DcxFileConstantEntry {
    DcxFileConstantEntryHeader header;
    Byte *constant_data;
};
```
* `constant_data`: C-style string.
* `constant_type`: If this constant is a class name, you should set this property to `CONSTANT_CLASS`, or it should be `CONSTANT_STRING`.
* `constant_data_size`: the length of string.

#### Class Pool
Class definitions are here.

Pool element:
```c++
struct DcxFileClassEntryHeader {
    UInt16 class_name_id;
    UInt16 parent_class_name_id;
    UInt16 class_slot_count;
    UInt16 member_slot_count;
};

struct DcxFileClassEntry {
    DcxFileClassEntryHeader header;
};
```
* `class_name_id`: Class name, a constant string.
* `parent_class_name_id`: Class name, a constant string.
* `class_slot_count`: Numbers of the slots which is in class.
* `member_slot_count`: Numbers of the slots which is in class instance(also called object).

#### Method Pool
Method definitions are here:

Pool element:
```c++
struct DcxFileMethodEntryHeader {
    UInt16 method_name_id;
    UInt16 method_signature_id;
    UInt16 method_return_type_name_id;
    UInt32 method_body_size;
    Bool method_is_native;
    Bool method_is_static;
};
struct DcxFileMethodEntry {
    DcxFileMethodEntryHeader header;
    Byte *method_body;
};
```
* `method_name_id`: Method name, a constant string.
* `method_signature_id`: Method signature, a constant string.
* `method_return_type_name_id`: Class name of the return type, a constant string.
* `method_length`: Size of the `method_body`.
* `method_body`: Method executable codes.
* `method_is_native`: Is native method or not.
* `method_is_static`: Is static method or not.
