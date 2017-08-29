## Dragon VM Executable Binary format

### Overview
DVM has its custom executable binary format, which is called `.dcx`

Each `.dcx` file contains the following information:
* DCX Magic
* Dragon VM Version ID

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

