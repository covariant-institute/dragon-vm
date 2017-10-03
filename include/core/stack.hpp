#pragma once

#include <core/type.hpp>
#include <core/memory.hpp>
#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>
#include <cstring>
#include <cstdlib>
#include <vector>

namespace dvm {
    namespace core {
        namespace object {
            class Object;

            class Method;
        }

        class Stack;

        class Frame final {
            friend class Stack;

        private:
            // 帧顶
            Byte *frame_limit;

            // 帧底
            Byte *base_pointer;

            // 原始的帧底，没有计算 shared 空间
            Byte *base_pointer_no_shared;

            // 当前位置
            Byte *sp;

            // 返回地址
            Byte *last_pc;

            // 在这个帧上的方法
            object::Method *method;

            // 帧的大小: base_pointer_no_shared - frame_limit
            SizeT frame_size;

            // 从上一个帧里分享的大小: base_pointer - base_pointer_no_shared
            SizeT shared_size;

            Frame(Byte *bp, SizeT size, SizeT shared);

            ~Frame();

            Byte *allocate(SizeT size);

            inline bool empty() const {
                return sp == base_pointer;
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
            inline Byte *get_last_pc() const {
                return last_pc;
            }

            inline void set_last_pc(Byte *last_pc) {
                this->last_pc = last_pc;
            }

            inline object::Method *get_method() const {
                return method;
            }

            inline void set_method(object::Method *method) {
                this->method = method;
            }

            template <typename T>
            inline T *at(UInt16 offset) {
                if (base_pointer - offset < frame_limit) {
                    throw dvm::core::Exception(DVM_MEMORY_STACK_INVALID_ACCESS);
                }
                return at<T>(base_pointer - offset);
            }

            template <typename T>
            inline void pop() {
                pop(sizeof(T));
            }

            inline void pop(SizeT size) {
                ensure_not_empty();
                sp += size;
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
                if (empty()) {
                    throw dvm::core::Exception(DVM_MEMORY_STACK_EMPTY);
                }
            }

        public:
            Stack() = default;

            Stack(const Stack &) = delete;

            explicit Stack(SizeT size);

            ~Stack();

            Frame *new_frame(SizeT size, SizeT shared = 0);

            void remove_top_frame();

            inline Frame *current_frame() const {
                ensure_has_frame();
                return frames[frames.size() - 1];
            }

            inline bool empty() const {
                return frames.empty();
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

            inline void pop(SizeT size) {
                current_frame()->pop(size);
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
