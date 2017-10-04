//
// Created by kiva on 2017/9/25.
//

#include <cstdio>
#include <core/type.hpp>
#include <core/runtime/dvm.hpp>
#include <core/runtime/thread.hpp>
#include <core/object/method.hpp>
#include <core/runtime/invoke.hpp>

int main(int argc, char **argv) {
    using namespace dvm::core;
    using namespace dvm::core::object;
    using namespace dvm::core::runtime;

    if (argc == 1) {
        printf("Usage: dvm-exec <bytecode-file>\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    auto bytes = static_cast<Byte *>(dvm_malloc(sizeof(Byte) * size));
    fread(bytes, static_cast<size_t>(size), 1, file);
    fclose(file);

    DragonVM dragonVM;
    Thread *thread = dragonVM.current_thread();
    VMContext *context = thread->get_context();

    MethodRegistry registry;
    registry.is_native = False;
    registry.is_static = True;
    registry.body_size = static_cast<UInt32>(size);
    registry.body = bytes;
    registry.locals_size = 256;
    registry.args_size = 0;
    Method::register_method(context, context->find_class("Void"), "main", "Void", registry);

    Method *method = context->resolve_method("main", "Void");
    Invocation::invoke_void(thread, method);

    dvm_free(bytes);
    return 0;
}
