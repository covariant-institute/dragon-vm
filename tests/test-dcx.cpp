//
// Created by kiva on 2017/9/8.
//

#include <core/config.hpp>
#include <core/dcx/dcx_file.hpp>
#include <core/dcx/dcx_linker.hpp>
#include <core/object/method.hpp>
#include <core/runtime/thread.hpp>
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
    // Try invoke our native method
    method->invoke(thread);

    method = context->resolve_method("dvm_main", "(X)");
    assert(method->is_native() == dvm::core::False);
    return 0;
}
