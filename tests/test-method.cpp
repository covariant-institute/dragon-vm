//
// Created by kiva on 2017/9/6.
//
#include <core/config.hpp>
#include <core/object/method.hpp>

using namespace dvm::core::object;
using namespace dvm::core::interpreter;

void ffi_hello(vm_context &context) {
    printf("Hello, this is ffi_hello");
}

int main() {
    auto *method = Method::new_foreign_method("ffi_hello", "()V", ffi_hello);
    vm_context context(dvm::core::config::STACK_DEFAULT_SIZE, dvm::core::config::HEAP_DEFAULT_SIZE);
    method->invoke(context);
    return 0;
}

