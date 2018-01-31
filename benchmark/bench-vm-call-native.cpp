//
// Created by kiva on 2017/9/17.
//

#include <core/runtime/context.hpp>
#include <core/object/method.hpp>
#include <core/runtime/invoke.hpp>
#include <sys/time.h>
#include "../experimental/calc-compiler/libdvm.h"

#define OPCODE(X) static_cast<dvm::core::runtime::VMOpcode>(dvm::core::runtime::VMOpcodes::X)

using namespace dvm::core;
using namespace dvm::core::object;
using namespace dvm::core::runtime;

static UInt16 constant_signature = 0;
static UInt16 constant_native_name = 1;
static UInt16 constant_vm_name = 2;

long diff_time_ms(struct timeval *start, struct timeval *end) {
    return (1000000 * (end->tv_sec - start->tv_sec) + end->tv_usec - start->tv_usec) / 1000;
}

extern "C" void Dragon_native_get_int_Int32(Thread *thread) {
    Invocation::return_from_method<Int32>(thread, 100);
}

extern "C" void Dragon_call_vm_get_int_Int32(Thread *thread) {
    auto method = thread->get_context()->resolve_method("vm_get_int", "Int32");
    auto ret = Invocation::invoke_get_result<Int32>(thread, method);
    Invocation::return_from_method<Int32>(thread, ret);
}

void register_native_get_int_method(VMContext *context) {
    MethodRegistry registry;
    registry.is_static = True;
    registry.is_native = True;
    registry.locals_size = sizeof(Int32);
    registry.args_size = 0;

    Method::register_method(context, context->find_class("Int32"),
                            "native_get_int", "Int32", registry);
}

void register_native_call_vm_get_int_method(VMContext *context) {
    MethodRegistry registry;
    registry.is_static = True;
    registry.is_native = True;
    registry.locals_size = sizeof(Int32);
    registry.args_size = 0;

    Method::register_method(context, context->find_class("Int32"),
                            "call_vm_get_int", "Int32", registry);
}

void register_vm_get_int_method(VMContext *context) {
    static Byte method_body[] = {
            OPCODE(ldc_i32), 0, 0, 0, 100,
            OPCODE(ret_i32)
    };
    MethodRegistry registry;
    registry.args_size = 0;
    registry.locals_size = sizeof(Int32);
    registry.is_static = True;
    registry.is_native = False;
    registry.body = method_body;
    registry.body_size = sizeof(method_body) / sizeof(method_body[0]);

    Method::register_method(context, context->find_class("Int32"),
                            "vm_get_int", "Int32", registry);
}

void register_vm_call_native_get_int_method(VMContext *context) {
    static Byte method_body[] = {
            OPCODE(invoke_method), 0, static_cast<Byte>(constant_native_name), 0, static_cast<Byte>(constant_signature),
            OPCODE(ret_i32)
    };
    MethodRegistry registry;
    registry.args_size = 0;
    registry.locals_size = sizeof(Int32) * 2 + sizeof(Int16) * 2;
    registry.is_static = True;
    registry.is_native = False;
    registry.body = method_body;
    registry.body_size = sizeof(method_body) / sizeof(method_body[0]);

    Method::register_method(context, context->find_class("Int32"),
                            "call_native_get_int", "Int32", registry);
}

void bench_native_call_vm(Thread *thread) {
    VMContext *context = thread->get_context();

    struct timeval begin{ }, end{ };
    gettimeofday(&begin, nullptr);

    Int32 ret = 0;
    auto method = context->resolve_method("call_vm_get_int", "Int32");
    for (int i = 0; i < 100000; ++i) {
        ret = Invocation::invoke_get_result<Int32>(thread, method);
    }

    gettimeofday(&end, nullptr);

    printf("Result: %d\n", ret);
    printf("Spent (100000 native->vm calls): %ld\n", diff_time_ms(&begin, &end));
}

void bench_vm_call_native(Thread *thread) {
    VMContext *context = thread->get_context();

    struct timeval begin{ }, end{ };
    gettimeofday(&begin, nullptr);

    Int32 ret = 0;
    auto method = context->resolve_method("call_native_get_int", "Int32");
    for (int i = 0; i < 100000; ++i) {
        ret = Invocation::invoke_get_result<Int32>(thread, method);
    }
    gettimeofday(&end, nullptr);

    printf("Result: %d\n", ret);
    printf("Spent (100000 vm->native calls): %ld\n", diff_time_ms(&begin, &end));
}

int main() {
    DragonVM vm;
    Thread *thread = vm.current_thread();
    VMContext *context = thread->get_context();

    context->register_constant(constant_native_name, "native_get_int");
    context->register_constant(constant_vm_name, "vm_get_int");
    context->register_constant(constant_signature, "Int32");

    register_native_call_vm_get_int_method(context);
    register_native_get_int_method(context);
    register_vm_call_native_get_int_method(context);
    register_vm_get_int_method(context);

    bench_native_call_vm(thread);
    bench_vm_call_native(thread);
}
