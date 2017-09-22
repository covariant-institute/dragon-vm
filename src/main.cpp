#include <core/dvm.hpp>

int main(int argc, const char **argv)
{
    try {
        return vm_main(argc, argv);
    } catch (const dvm::core::Exception &e) {
        fprintf(stderr, "DragonVM: <Error>: %s\n", e.what());
        return 1;
    }
}
