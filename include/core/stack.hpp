#pragma once

#include <core/type.hpp>
#include <core/memory.hpp>
#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>
#include <core/object/object.hpp>
#include <cstring>
#include <cstdlib>
#include <vector>

namespace dvm {
    namespace core {
        class Stack;

        class Frame final {
            friend class Stack;

        private:
            Byte *bp;
            Byte *sp;
            Byte *sl;
            SizeT frame_size;

            Frame(Byte *bp, SizeT size);

            ~Frame();

            Byte *allocate(SizeT size);

            inline bool empty() const {
                return sp == bp;
            }

            inline void ensure_not_empty() const {
                if (empty()) {
                    throw dvm::core::Exception(DVM_MEMORY_STACK_EMPTY);
                }
            }

            template <typename T>
            inline T *at(Byte *bytes) const {
                return reinterpret_cast<T *>(bytes);
            }

        public:
            template <typename T>
            inline T *at(UInt16 offset) {
                if (bp - offset < sl) {
                    throw dvm::core::Exception(DVM_MEMORY_STACK_INVALID_ACCESS);
                }
                return at<T>(bp - offset);
            }

            template <typename T>
            inline void pop() {
                ensure_not_empty();
                sp += sizeof(T);
            }

            template <typename T>
            inline void push(T &&t) {
                Byte *byte = allocate(sizeof(T));
                *at<T>(byte) = std::forward<T>(t);
            }

            template <typename T>
            inline T peek() const {
                ensure_not_empty();
                return *at<T>(sp);
            }

            template <typename T>
            inline T peek_pop() {
                T ret = peek<T>();
                pop<T>();
                return ret;
            }

            inline void pop_object() {
                pop<object::Object *>();
            }

            inline object::Object *peek_object() const {
                ensure_not_empty();
                auto **ref = at<object::Object *>(sp);
                return ref != nullptr ? *ref : nullptr;
            }

            inline object::Object *peek_object_pop() {
                object::Object *ret = peek_object();
                pop_object();
                return ret;
            }

            inline void push_object_ref(object::Object *obj) {
                // push the address of the object as a reference
                Byte *ref = allocate(sizeof(object::Object *));
                *at<object::Object *>(ref) = obj;
            }
        };

        class Stack final {
        private:
            // stack start
            Byte *memory = nullptr;

            // stack pointer
            Byte *sp = nullptr;

            // frames
            std::vector<Frame *> frames;

            inline void ensure_has_frame() const {
                if (frames.empty()) {
                    throw dvm::core::Exception(DVM_MEMORY_STACK_EMPTY);
                }
            }

        public:
            Stack() = default;

            Stack(const Stack &) = delete;

            explicit Stack(SizeT size);

            ~Stack();

            void new_frame(SizeT size);

            void remove_top_frame();

            inline Frame *current_frame() const {
                ensure_has_frame();
                return frames[frames.size() - 1];
            }

            template <typename T>
            inline T *at(UInt16 offset) {
                return current_frame()->at<T>(offset);
            }

            inline object::Object **at_object(UInt16 offset) {
                return current_frame()->at<object::Object *>(offset);
            }

            template <typename T>
            inline void pop() {
                current_frame()->pop<T>();
            }

            template <typename T>
            inline void push(T &&t) {
                current_frame()->push<T>(std::forward<T>(t));
            }

            template <typename T>
            inline T peek() const {
                return current_frame()->peek<T>();
            }

            template <typename T>
            inline T peek_pop() {
                return current_frame()->peek_pop<T>();
            }

            inline void pop_object() {
                current_frame()->pop_object();
            }

            inline object::Object *peek_object() const {
                return current_frame()->peek_object();
            }

            inline object::Object *peek_object_pop() {
                return current_frame()->peek_object_pop();
            }

            inline void push_object_ref(object::Object *obj) {
                current_frame()->push_object_ref(obj);
            }
        };
    }
}
