//
// Created by kiva on 2017/9/6.
//
#include <core/config.hpp>
#include <core/object/method.hpp>

using namespace dvm::core::object;
using namespace dvm::core::runtime;

extern "C" void dvm_native_hello(VMContext &context) {
    printf("In dvm_native_hello(): Hello, Dragon VM\n");
}

int main() {
    DragonVM vm;
    Thread *thread = vm.current_thread();
    VMContext *context = thread->get_context();

    Method::register_native_method(context, context->find_class("Int32"),
                                   "dvm_native_hello", "()", dvm::core::False, 128);

    auto method = context->resolve_method("dvm_native_hello", "()");
    method->invoke(thread);
    return 0;
}

