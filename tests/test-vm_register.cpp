//
// Created by kiva on 2017/8/30.
//

#include <core/interpreter/vm_register.hpp>
#include <core/object/basic_classes.hpp>
#include <cassert>
#include <cmath>

int main() {
    using namespace dvm::core::interpreter::registers;
    using namespace dvm::core::object;
    init_base_classes();

    vm_register_holder regs;

    auto i32 = Class::find_class("Int32")->create_object();
    i32->slots[1].slot_type = dvm::core::type_identifier::TYPE_ID_INT32;
    i32->slots[1].i32 = 10086;
    regs.get_register(vm_register_id::VM_REG_R0).set(i32);

    assert(regs.get_register<vm_register_id::VM_REG_R0>().get()->slots[1].i32 == 10086);
}
