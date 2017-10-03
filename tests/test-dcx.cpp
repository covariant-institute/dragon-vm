//
// Created by kiva on 2017/9/8.
//

#include <core/config.hpp>
#include <core/dcx/dcx_file.hpp>
#include <core/dcx/dcx_linker.hpp>
#include <core/object/method.hpp>
#include <core/runtime/dispatcher.hpp>
#include <cassert>

using namespace dvm::core::dcx;
using namespace dvm::core::runtime;
using namespace dvm::core::object;

extern "C" void Dragon_dvm_main0_Void(Thread *thread) {
    printf("In dvm_main(): Hello, Dragon VM\n");
}

int main(int argc, const char **argv) {

    auto dcx = DcxFile::open(argv[1] ? argv[1] : "../cmake-build-debug/empty-dcx.dcx");
    DragonVM vm;
    Thread *thread = vm.current_thread();
    VMContext *context = thread->get_context();
    DcxLinker linker;
    linker.link(context, dcx);

    auto obj = context->find_class("Main")->new_instance();
    assert(obj != nullptr);

    auto method = context->resolve_method("dvm_main0", "Void");
    assert(method->is_native() == dvm::core::True);
    Method::dump_method_info(method);
    Invocation::invoke_simple(thread, method, 0);
    thread->get_stack().remove_top_frame();

    method = context->resolve_method("add_two_int32", "Int32_Int32_Int32");
    assert(method->is_native() == dvm::core::False);
    Method::dump_method_info(method);

    method = context->resolve_method("dvm_main", "Void");
    assert(method->is_native() == dvm::core::False);
    Method::dump_method_info(method);

    // call dvm_main
    Invocation::invoke_simple(thread, method, sizeof(dvm::core::Int32));
    auto ret = thread->get_stack().peek_pop<dvm::core::Int32>();
    printf("Method returned: %d\n", ret);
    assert(ret == 19);
    thread->get_stack().remove_top_frame();

    return 0;
}
