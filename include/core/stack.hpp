#pragma once

#include <core/type.hpp>
#include <core/memory.hpp>
#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>
#include <core/object/object.hpp>
#include <cstring>
#include <cstdlib>

namespace dvm {
    namespace core {
        class Stack final {
        private:
            // stack start
            void *memory = nullptr;

            // stack pointer
            Byte *sp = nullptr;

            // frame pointer
            Byte *bp = nullptr;

            // stack limit
            Byte *sl = nullptr;

            inline void ensure_stack_not_empty() const {
                if (empty()) {
                    throw dvm::core::exception(DVM_MEMORY_STACK_EMPTY);
                }
            }

            object::Object *allocate_on_stack(SizeT size);

        public:
            Stack() = delete;

            Stack(const Stack &) = delete;

            explicit Stack(SizeT size);

            ~Stack();

            inline bool empty() const {
                return sp == sl;
            }

            object::Object *peek();

            void pop();

            object::Object *new_instance(const object::Class *prototype);
        };
    }
}
