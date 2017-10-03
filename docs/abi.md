## DragonVM Application Binary Interface

### Native Method

Native method's name should follow these rules:
* **Starts with** `Dragon`, **followed** by the `mangled string`.
* There's only a single parameter `dvm::core::runtime::Thread *`
* Each unit is separated by `_`.

Mangled string should follow these rules:
* Primitive types remain themselves, e.g. `Int32` -> `Int32`,
  `Float` -> `Float`.
* `Void` type is named `Void`
* Array types has a `A` prefix, e.g. `Int32[]` -> `AInt32`,
  `Double[]` -> `ADouble`.
* Object types are named `Object`, e.g. `SomeClass` -> `Object`.
* Constructed with `ReturnType_MethodName_ArgumentTypes`, 
  e.g. a method named `hello` accepting  two `Int32` 
  and returns a `Float` -> `Object_hello_Int32_Int32`

For example, we have a native method: 
`Double divide(Int32, Int32)`, the native function's name 
(`C/C++` function name) of which is: `void Dragon_Double_divide_Int32_Int32(Thread *)`.

More examples:

`void hello_native()` -> `void Dragon_Void_hello_native(Thread *)`

`SomeClass get_one(String name)` -> `void Dragon_Object_get_one_Object(Thread *)`