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

    VMContext context{};

    VMRegisterHolder regs;

    auto i32 = Class::find_class(context, "Int32")->new_instance();
    i32->slots[1].slot_type = dvm::core::type_identifier::TYPE_ID_INT32;
    i32->slots[1].i32 = 10086;
    regs.get_register(VMRegisterName::R0).set(i32);

    assert(regs.get_register<VMRegisterName::R0>().get()->slots[1].i32 == 10086);
}
