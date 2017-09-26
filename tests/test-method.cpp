//
// Created by kiva on 2017/9/6.
//
#include <core/config.hpp>
#include <core/object/method.hpp>
#include <core/runtime/dispatcher.hpp>

using namespace dvm::core::object;
using namespace dvm::core::runtime;

extern "C" void dvm_native_hello(Thread *thread) {
    printf("In dvm_native_hello(): Hello, Dragon VM\n");
    thread->get_stack().push<dvm::core::Double>(3.1415926);
}

int main() {
    DragonVM vm;
    Thread *thread = vm.current_thread();
    VMContext *context = thread->get_context();

    Method::register_native_method(context, context->find_class("Double"),
                                   "dvm_native_hello", "()", dvm::core::False, 128);

    auto method = context->resolve_method("dvm_native_hello", "()");
    Dispatcher::invoke_method(thread, method);
    return 0;
}

