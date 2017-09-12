//
// Created by kiva on 2017/9/8.
//

#include <core/config.hpp>
#include <core/dcx/dcx_file.hpp>
#include <core/dcx/dcx_linker.hpp>
#include <core/object/basic_classes.hpp>
#include <core/runtime/vm_context.hpp>

int main(int argc, const char **argv) {
    using namespace dvm::core::dcx;
    using namespace dvm::core::runtime;
    using namespace dvm::core::object;
    VMContext context{};
    init_base_classes(context);

    auto dcx = DcxFile::open(argv[1] ? argv[1] : "../cmake-build-debug/empty-dcx.dcx");
    DcxLinker::link(context, dcx);

    auto obj = Class::find_class(context, "Main")->new_instance();
    printf("%s\n", obj->prototype->name->c_str());
    return 0;
}
