//
// Created by kiva on 2017/8/28.
//

#include <core/runtime/vm_context.hpp>
#include <cassert>

int main(int argc, const char **argv) {
    using namespace dvm::core::runtime;
    VMContext context1{ };
    VMContext context2{ };
    VMContext context3{ };

    assert(context1.find_class("Object") != nullptr);
    assert(context2.find_class("Object") != nullptr);
    assert(context3.find_class("Object") != nullptr);
}
