//
// Created by kiva on 2017/8/30.
//
#include <core/runtime/vm_register.hpp>
#include <core/memory.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
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

            RegisterVisitor VMRegisterHolder::operator[](VMRegisterID reg) {
                if (reg < VM_REGISTERS_NUMBER) {
                    return RegisterVisitor(registers + reg);
                }

                throw dvm::core::Exception(DVM_REGISTER_INVALID);
            }
        }
    }
}
