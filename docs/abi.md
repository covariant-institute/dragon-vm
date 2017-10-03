## DragonVM Application Binary Interface

### Method Signature

* Primitive types remain themselves, e.g. `Int32` -> `Int32`,
  `Float` -> `Float`.
* `Void` type is named `Void`
* Array types has a `A` prefix, e.g. `Int32[]` -> `AInt32`,
  `Double[]` -> `ADouble`.
* Object types are named `Object`, e.g. `SomeClass` -> `Object`.
* Constructed with `MethodName_ReturnType_ArgumentTypes`, 
  e.g. a method named `hello` accepting  two `Int32` 
  and returns a `Float` -> `hello_Object_Int32_Int32`
  
### Native Method

Native method's name should follow these rules:
* **Starts with** `Dragon`, **followed** by the `method signature`.
* There's only a single parameter `dvm::core::runtime::Thread *`
* Each unit is separated by `_`.

For example, we have a native method: 
`Double divide(Int32, Int32)`, the native function's name 
(`C/C++` function name) of which is: `void Dragon_divide_Double_Int32_Int32(Thread *)`.

More examples:

`void hello_native()` -> `void Dragon_hello_native_Void(Thread *)`

`SomeClass get_one(String name)` -> `void Dragon_get_one_Object_Object(Thread *)`