//
// Created by kiva on 2017/9/6.
//
#include <core/config.hpp>
#include <core/object/method.hpp>
#include <cassert>

using namespace dvm::core::object;
using namespace dvm::core::runtime;

extern "C" void dvm_native_hello(VMContext &context) {
    printf("In dvm_native_hello(): Hello, Dragon VM\n");
}

int main() {
    VMContext context{ };
    Method::register_native_method(context, context.find_class("Int32"),
                                   "dvm_native_hello", "()", dvm::core::False);

    auto method = context.resolve_method("dvm_native_hello", "()");
    method->invoke(context);
    return 0;
}

