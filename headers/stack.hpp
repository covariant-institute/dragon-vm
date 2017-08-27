#pragma once

#include "type.hpp"
#include <cstring>
#include <cstdlib>

namespace dvm {
    struct mem_blck_info final {
        UInt type_code;
        Addr data_size;
    };

    class stack final {
        void *allocated_start = nullptr;
        Byte *stack_pointer = nullptr;

    private:
        inline void move_forward(size_t forward_size) {
            stack_pointer += forward_size;
        }

        inline void move_backward(size_t backward_size) {
            stack_pointer -= backward_size;
        }

    public:
        stack() = delete;

        explicit stack(ULong s) : allocated_start(malloc(s)) {
            stack_pointer = static_cast<Byte *>(allocated_start);
        }

        stack(const stack &) = delete;

        ~stack() {
            free(allocated_start);
        }

        void push(const mem_blck_info &mbi, void *dat) {
            memcpy(stack_pointer, dat, mbi.data_size);
            move_forward(mbi.data_size);
            memcpy(stack_pointer, (void *) &mbi, sizeof(mem_blck_info));
            move_forward(sizeof(mem_blck_info));
        }

        void pop() {
            move_backward(sizeof(mem_blck_info));
            mem_blck_info *mbi = reinterpret_cast<mem_blck_info *>(stack_pointer);
            move_backward(mbi->data_size);
        }

        void *top() {
            void *target_addr = stack_pointer - sizeof(mem_blck_info);
            mem_blck_info *mbi = reinterpret_cast<mem_blck_info *>(target_addr);
            return stack_pointer - sizeof(mem_blck_info) - mbi->data_size;
        }
    };
}
