//
// Created by kiva on 2017/9/6.
//
#include <core/config.hpp>
#include <core/object/method.hpp>
#include <cassert>

using namespace dvm::core::object;
using namespace dvm::core::runtime;

extern "C" Object *dvm_native_hello(VMContext &context) {
    printf("In dvm_native_hello(): Hello, Dragon VM\n");
    return context.new_Int32(122);
}

int main() {
    VMContext context{};
    Method::register_native_method(context, context.find_class("Int32"),
                                   "dvm_native_hello", "()", dvm::core::False);

    auto method = context.resolve_method("dvm_native_hello", "()");
    auto ret = method->invoke(context);
    assert(ret);
    assert(ret->slots[1].get<dvm::core::Int32>() == 122);
    return 0;
}

