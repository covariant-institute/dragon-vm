//
// Created by kiva on 2017/8/30.
//

#pragma once

#include <core/vm_register_common.hpp>

namespace dvm {
    namespace core {
        namespace vm {
            namespace registers {
                class register_visitor {
                private:
                    Link register_addr;

                public:
                    register_visitor(const register_visitor &other) {
                        this->register_addr = other.register_addr;
                    }

                    explicit register_visitor(Link addr)
                            : register_addr(addr) { }

                    ~register_visitor() = default;

                    template <typename T>
                    T get() {
                        return *reinterpret_cast<T *>(register_addr);
                    }

                    template <typename T>
                    T *set(T value) {
                        T *ptr = reinterpret_cast<T *>(register_addr);
                        *ptr = value;
                        return ptr;
                    }
                };
            }
        }
    }
}
