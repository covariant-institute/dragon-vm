//
// Created by kiva on 2017/9/3.
//
#pragma once

#include <core/runtime/register_common.hpp>
#include <core/object/object.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            class RegisterVisitor {
            private:
                VMRegister *register_ptr;

            public:
                RegisterVisitor(const RegisterVisitor &);

                explicit RegisterVisitor(VMRegister *register_ptr);

                ~RegisterVisitor() = default;

                VMRegister *operator->() {
                    return register_ptr;
                }
            };
        }
    }
}
