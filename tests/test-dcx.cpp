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

extern "C" void dvm_main(VMContext &context) {
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

    auto method = context->resolve_method("dvm_main", "(N)");
    assert(method->is_native() == dvm::core::True);
    Method::dump_method_info(method);
    // Try invoke our native method
    Dispatcher::invoke_method(thread, method);

    method = context->resolve_method("dvm_main", "(X)");
    assert(method->is_native() == dvm::core::False);
    Method::dump_method_info(method);

    thread->get_stack().new_frame(sizeof(dvm::core::Int32));
    Dispatcher::invoke_method(thread, method);
    auto ret = thread->get_stack().peek_pop<dvm::core::Int32>();
    printf("Method returned: %d\n", ret);
    assert(ret == 1);
    thread->get_stack().remove_top_frame();

    return 0;
}
