//
// Created by kiva on 2017/9/6.
//
#include <core/config.hpp>
#include <core/object/method.hpp>
#include <core/runtime/dispatcher.hpp>
#include <core/dcx/dcx_file_info.hpp>
#include <cassert>

using namespace dvm::core;
using namespace dvm::core::object;
using namespace dvm::core::runtime;

extern "C" void dvm_native_hello(Thread *thread) {
    printf("In dvm_native_hello(): Hello, Dragon VM\n");
    thread->get_stack().push<Double>(3.14159);
    Invocation::return_stack_top<Double>(thread);
}

int main() {
    DragonVM vm;
    Thread *thread = vm.current_thread();
    VMContext *context = thread->get_context();

    dcx::DcxFileMethodEntry methodEntry{ };
    methodEntry.header.method_is_static = True;
    methodEntry.header.method_is_native = True;
    methodEntry.header.method_handlers_count = 0;
    methodEntry.header.method_locals_size = sizeof(Double);
    methodEntry.header.method_args_size = 0;

    Method::register_method(context, context->find_class("Double"),
                            "dvm_native_hello", "()", methodEntry);

    auto method = context->resolve_method("dvm_native_hello", "()");
    Invocation::invoke_simple(thread, method, sizeof(Double));
    auto ret = thread->get_stack().peek_pop<Double>();
    printf("Native method returned: %lf\n", ret);
    assert(ret == 3.14159);
    thread->get_stack().remove_top_frame();
    return 0;
}

