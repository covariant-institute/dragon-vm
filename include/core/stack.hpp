#pragma once

#include <core/type.hpp>
#include <core/memory.hpp>
#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>
#include <cstring>
#include <cstdlib>

namespace dvm {
    namespace core {
        class Stack final {
        private:
            // stack start
            void *ss = nullptr;

            // stack pointer
            Byte *sp = nullptr;

            // stack limit
            Byte *sl = nullptr;

            inline void ensure_stack_not_empty() const {
                if (empty()) {
                    throw dvm::core::exception(DVM_MEMORT_STACK_EMPTY);
                }
            }

        public:
            Stack() = delete;

            Stack(const Stack &) = delete;

            explicit Stack(SizeT size);

            ~Stack();

            inline bool empty() const {
                return sp == sl;
            }

            void *top();

            void *adjust(SizeT size);

            void pop();

            template <typename T>
            void push(const T &t) {
                auto *element = static_cast<T *>(adjust(sizeof(T)));
                dvm_construct_object(element, t);
            }

            template <typename T>
            void push_array(const T *array, SizeT length) {
                auto *elements = static_cast<T *>(adjust(sizeof(T) * length));
                dvm_construct_object_array(elements, array, length);
            }

            inline SizeT size_of(void *ptr) {
                return *reinterpret_cast<SizeT *>(reinterpret_cast<Byte *>(ptr) - sizeof(SizeT));
            }
        };
    }
}
