//
// Created by kiva on 2017/8/30.
//
#pragma once

#include <core/runtime/register_common.hpp>
#include <core/runtime/register_visitor.hpp>
#include <core/exceptions.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            class VMContext;

            class VMRegisterHolder {
                friend class VMContext;

            private:
                VMRegister *registers;

            public:
                VMRegisterHolder(const VMRegisterHolder &) = delete;

                VMRegisterHolder();

                ~VMRegisterHolder();

                RegisterVisitor operator[](VMRegisterID id);
            };
        }
    }
}
