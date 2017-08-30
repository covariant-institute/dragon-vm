//
// Created by kiva on 2017/8/30.
//

#include <core/vm_register.hpp>
#include <cassert>
#include <cmath>

int main() {
    using namespace dvm::core::vm::registers;
    vm_register_holder regs;

    // ix7 = 100
    regs.get_register(vm_register_id::VM_REG_IX7).set<VMRegisterIX32>(100);

    // ix8 = INT64_MAX - 100
    regs.get_register(vm_register_id::VM_REG_IX8).set<VMRegisterIX64>(INT64_MAX - 100);

    auto ix7 = regs.get_register<vm_register_id::VM_REG_IX7>().get<VMRegisterIX32>();
    auto ix8 = regs.get_register<vm_register_id::VM_REG_IX8>().get<VMRegisterIX64>();

    printf("ix7= %d, ix8 = %lld, sum = %lld\n", ix7, ix8, ix7 + ix8);
    assert(ix7 + ix8 == INT64_MAX);

    // fx0 = 3.14
    regs.get_register<vm_register_id::VM_REG_FX0>().set<VMRegisterFX32>(3.14);

    // fx2 = 1.1234
    regs.get_register<vm_register_id::VM_REG_FX2>().set<VMRegisterFX64>(1.1234);

    auto fx0 = regs.get_register<vm_register_id::VM_REG_FX0>().get<VMRegisterFX32>();
    auto fx2 = regs.get_register<vm_register_id::VM_REG_FX2>().get<VMRegisterFX64>();
    printf("fx0 = %f, fx2 = %.12lf, sum = %.12lf\n", fx0, fx2, fx0 + fx2);
    assert(fabs(fx0 + fx2 - 4.2634) < 0.0001);

    printf("abs(ix7 + fx0 - ix7 - fx0) = %.10f\n", fabsf(ix7 + fx0 - ix7 - fx0));
    assert(fabs(fabs(ix7 + fx0 - ix7) - fx0) < 0.01);
}
