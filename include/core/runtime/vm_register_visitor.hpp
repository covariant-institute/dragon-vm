//
// Created by kiva on 2017/9/3.
//
#pragma once

#include <core/runtime/vm_register_common.hpp>
#include <core/object/object.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            namespace registers {
                class RegisterVisitor {
                private:
                    VMRegister *register_ptr;

                public:
                    RegisterVisitor(const RegisterVisitor &);

                    explicit RegisterVisitor(VMRegister *register_ptr);

                    ~RegisterVisitor() = default;

                    void set(object::Object *object) {
                        *this->register_ptr = object;
                    }

                    object::Object* get() {
                        return *this->register_ptr;
                    }
                };
            }
        }
    }
}
