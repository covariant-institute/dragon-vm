//
// Created by kiva on 2017/8/30.
//
#include <core/interpreter/vm_register.hpp>
#include <core/interpreter/vm_register_offsets.hpp>
#include <core/errorcodes.hpp>
#include <cstdlib>

namespace dvm {
    namespace core {
        namespace interpreter {
            namespace registers {
                constexpr size_t get_registers_memory_size() {
                    return sizeof(VMRegisterIX64) * VM_REGISTERS_NUMBER;
                }

                vm_register_holder::vm_register_holder() throw(dvm::core::exception::exception) : registers(nullptr) {
                    auto memory = static_cast<Link>(::malloc(get_registers_memory_size()));
                    if (memory == nullptr) {
                        throw dvm::core::exception::exception(DVM_BAD_ALLOC);
                    }
                    this->registers = memory;
                }

                vm_register_holder::~vm_register_holder() {
                    delete registers;
                }

                register_visitor vm_register_holder::get_register(vm_register_id register_identifier) {
                    auto id = static_cast<VMRegisterID>(register_identifier);

                    if (id < VM_REGISTERS_NUMBER) {
                        return register_visitor(registers + offsets::REGISTER_OFFSET_TABLE[id]);
                    }

                    throw dvm::core::exception::exception(DVM_REGISTER_INVALID);
                }
            }
        }
    }
}
