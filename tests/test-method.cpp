//
// Created by kiva on 2017/9/6.
//
#include <core/config.hpp>
#include <core/object/method.hpp>
#include <core/runtime/dispatcher.hpp>
#include <core/dcx/dcx_file_info.hpp>

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

    dvm::core::dcx::DcxFileMethodEntry methodEntry{ };
    methodEntry.header.method_is_static = dvm::core::True;
    methodEntry.header.method_is_native = dvm::core::True;
    methodEntry.header.method_handlers_count = 0;
    methodEntry.header.method_locals_size = sizeof(dvm::core::Double);
    methodEntry.header.method_args_size = 0;

    Method::register_method(context, context->find_class("Double"),
                            "dvm_native_hello", "()", methodEntry);

    auto method = context->resolve_method("dvm_native_hello", "()");
    Dispatcher::invoke_method(thread, method);
    return 0;
}

