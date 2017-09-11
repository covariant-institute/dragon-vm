//
// Created by kiva on 2017/8/30.
//
#include <core/runtime/vm_register.hpp>
#include <core/memory.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            namespace registers {
                constexpr size_t get_registers_memory_size() {
                    return sizeof(VMRegister) * VM_REGISTERS_NUMBER;
                }

                vm_register_holder::vm_register_holder() : registers(nullptr) {
                    auto memory = static_cast<VMRegister *>(dvm_malloc(get_registers_memory_size()));
                    this->registers = memory;
                }

                vm_register_holder::~vm_register_holder() {
                    dvm_free(registers);
                }

                register_visitor vm_register_holder::get_register(vm_register_id register_identifier) {
                    auto id = static_cast<VMRegisterID>(register_identifier);

                    if (id < VM_REGISTERS_NUMBER) {
                        return register_visitor(registers + id);
                    }

                    throw dvm::core::exception(DVM_REGISTER_INVALID);
                }
            }
        }
    }
}
