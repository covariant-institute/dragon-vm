//
// Created by kiva on 2017/9/6.
//
#include <core/config.hpp>
#include <core/object/method.hpp>
#include <core/dcx/dcx_file.hpp>

using namespace dvm::core::object;
using namespace dvm::core::runtime;

void ffi_hello(VMContext &context) {
    printf("Hello, this is ffi_hello\n");
}

int main() {
    auto *method = Method::new_foreign_method("ffi_hello", "()V", ffi_hello);
    VMContext context{ };
    method->invoke(context);
    return 0;
}

