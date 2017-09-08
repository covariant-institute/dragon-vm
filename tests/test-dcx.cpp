//
// Created by kiva on 2017/9/8.
//

#include <core/config.hpp>
#include <core/dcx/dcx_reader.hpp>
#include <cassert>

int main(int argc, const char **argv) {
    using namespace dvm::core::dcx;
    dcx_reader reader;
    assert(reader.open(argv[1] ? argv[1] : "../build-test/empty-dcx.dcx"));

    reader.close();
}
