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
                class register_visitor {
                private:
                    VMRegister *register_ptr;

                public:
                    register_visitor(const register_visitor &);

                    explicit register_visitor(VMRegister *register_ptr);

                    ~register_visitor() = default;

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
