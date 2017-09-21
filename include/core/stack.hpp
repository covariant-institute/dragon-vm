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
                    throw dvm::core::Exception(DVM_MEMORY_STACK_EMPTY);
                }
            }

            Byte *allocate_on_stack(SizeT size);

        public:
            Stack() = default;

            Stack(const Stack &) = delete;

            explicit Stack(SizeT size);

            ~Stack();

            inline bool empty() const {
                return sp == sl;
            }

            void pop() {
                ensure_stack_not_empty();
                sp += *reinterpret_cast<SizeT *>(sp) + sizeof(SizeT);
            }

            template <typename T>
            void push(T &&t) {
                Byte *byte = allocate_on_stack(sizeof(T));
                *reinterpret_cast<T *>(byte) = std::forward<T>(t);
            }

            template <typename T>
            T peek() const {
                ensure_stack_not_empty();
                return *reinterpret_cast<T *>(sp + sizeof(SizeT));
            }


            object::Object *peek_object() const {
                ensure_stack_not_empty();
                auto **ref = reinterpret_cast<object::Object **>(sp + sizeof(SizeT));
                return ref != nullptr ? *ref : nullptr;
            }

            void push_object_ref(object::Object *obj) {
                // push the address of the object as a reference
                Byte *ref = allocate_on_stack(sizeof(object::Object *));
                *reinterpret_cast<object::Object **>(ref) = obj;
            }

//            object::Object *peek_object() const {
//                ensure_stack_not_empty();
//                return reinterpret_cast<object::Object *>(sp + sizeof(SizeT));
//            }

//            object::Object *new_object(const object::Class *prototype);
        };
    }
}
