//
// Created by kiva on 2017/10/15.
//
#include <core/object/box.hpp>
#include <cassert>

using namespace dvm::core;
using namespace dvm::core::object;
using namespace dvm::core::runtime;

int main() {
    DragonVM vm;
    VMContext *context = vm.current_thread()->get_context();

    auto i32 = box(context, 1008611);
    assert(unbox<Int32>(i32) == 1008611);
}
