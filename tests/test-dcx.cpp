//
// Created by kiva on 2017/9/8.
//

#include <core/config.hpp>
#include <core/dcx/dcx_reader.hpp>
#include <cassert>

int main(int argc, const char **argv) {
    using namespace dvm::core::dcx;
    DcxReader reader { };
    assert(reader.open(argv[1] ? argv[1] : "../cmake-build-debug/empty-dcx.dcx"));

    DcxFileConstantEntry constantEntries[reader.get_constant_entries()];

    DcxFileClassEntry classEntries[reader.get_class_entries()];

    DcxFileMethodEntry methodEntries[reader.get_method_entries()];

    for (int i = 0; i < reader.get_constant_entries(); ++i) {
        reader.read_next_constant_entry(constantEntries[i]);
    }

    for (int i = 0; i < reader.get_class_entries(); ++i) {
        reader.read_next_class_entry(classEntries[i]);
    }

    for (int i = 0; i < reader.get_method_entries(); ++i) {
        reader.read_next_method_entry(methodEntries[i]);
    }

    reader.close();
}
