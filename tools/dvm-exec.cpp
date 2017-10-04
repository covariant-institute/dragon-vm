//
// Created by kiva on 2017/9/25.
//

#include <cstdio>
#include <core/type.hpp>
#include <core/runtime/dvm.hpp>
#include <core/runtime/thread.hpp>
#include <core/dcx/dcx_file_info.hpp>

int main(int argc, char **argv) {
    using namespace dvm::core;
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


    thread->get_stack().new_frame(256);
    thread->set_runnable(bytes);
    thread->run();
    thread->get_stack().remove_top_frame();

    dvm_free(bytes);
    return 0;
}
