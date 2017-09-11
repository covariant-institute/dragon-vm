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

                VMRegisterHolder::VMRegisterHolder() : registers(nullptr) {
                    auto memory = static_cast<VMRegister *>(dvm_malloc(get_registers_memory_size()));
                    this->registers = memory;
                }

                VMRegisterHolder::~VMRegisterHolder() {
                    dvm_free(registers);
                }

                RegisterVisitor VMRegisterHolder::get_register(VMRegisterName register_identifier) {
                    auto id = static_cast<VMRegisterID>(register_identifier);

                    if (id < VM_REGISTERS_NUMBER) {
                        return RegisterVisitor(registers + id);
                    }

                    throw dvm::core::exception(DVM_REGISTER_INVALID);
                }
            }
        }
    }
}
