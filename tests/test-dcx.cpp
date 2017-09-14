//
// Created by kiva on 2017/9/8.
//

#include <core/config.hpp>
#include <core/dcx/dcx_file.hpp>
#include <core/object/method.hpp>
#include <core/runtime/vm_context.hpp>

int main(int argc, const char **argv) {
    using namespace dvm::core::dcx;
    using namespace dvm::core::runtime;
    using namespace dvm::core::object;

    auto dcx = DcxFile::open(argv[1] ? argv[1] : "../cmake-build-debug/empty-dcx.dcx");
    VMContext context(dcx);

    auto obj = context.find_class("Main")->new_instance();
    printf("%s\n", obj->prototype->name->c_str());

    auto method = context.resolve_method("dvm_main", "(K)");
    printf("%s @ %s, is_native: %s\n", method->get_name().c_str(), method->get_signature().c_str(),
           method->is_native() ? "true" : "false");

    method = context.resolve_method("dvm_main", "(X)");
    printf("%s @ %s, is_native: %s\n", method->get_name().c_str(), method->get_signature().c_str(),
           method->is_native() ? "true" : "false");
    return 0;
}
