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

extern "C" void Dragon_dvm_native_hello_Double(Thread *thread) {
    Invocation::return_from_method<Double>(thread, 3.14159);
}

int main() {
    DragonVM vm;
    Thread *thread = vm.current_thread();
    VMContext *context = thread->get_context();

    MethodRegistry registry;
    registry.is_static = True;
    registry.is_native = True;
    registry.locals_size = sizeof(Double);
    registry.args_size = 0;

    Method::register_method(context, context->find_class("Double"),
                            "dvm_native_hello", "Double", registry);

    auto method = context->resolve_method("dvm_native_hello", "Double");
    auto ret = Invocation::invoke_get_result<Double>(thread, method);
    assert(ret == 3.14159);
    return 0;
}

