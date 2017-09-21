//
// Created by kiva on 2017/8/30.
//

#include <core/runtime/vm_register.hpp>
#include <cassert>
#include <core/runtime/vm_context.hpp>

int main() {
    using namespace dvm::core::runtime::registers;
    using namespace dvm::core::object;
    using namespace dvm::core::runtime;

    VMContext context{ };

    VMRegisterHolder regs;

    auto i32 = context.find_class("Int32")->new_instance();
    i32->slots[1].set(10086);

    auto visitor = regs.get_register(VMRegisterName::R0);
    visitor->set(10086);

    assert(visitor->data.i32 == 10086);
}
