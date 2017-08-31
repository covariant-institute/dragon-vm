//
// Created by kiva on 2017/8/30.
//

#pragma once

#include <core/interpreter/vm_register_common.hpp>

namespace dvm {
    namespace core {
        namespace vm {
            namespace registers {
                class register_visitor {
                private:
                    Link register_addr;

                    template <typename T>
                    inline T* get_pointer() {
                        return reinterpret_cast<T *>(register_addr);
                    }

                public:
                    register_visitor(const register_visitor &other);

                    explicit register_visitor(Link addr);

                    ~register_visitor() = default;

                    template <typename T>
                    T get() {
                        return *get_pointer<T>();
                    }

                    template <typename T>
                    T *set(T value) {
                        T *ptr = get_pointer<T>();
                        *ptr = value;
                        return ptr;
                    }
                };
            }
        }
    }
}
