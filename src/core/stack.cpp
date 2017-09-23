//
// Created by kiva on 2017/9/10.
//

#include <core/stack.hpp>

namespace dvm {
    namespace core {

        Stack::~Stack() {
            frames.clear();
            dvm_free(memory);
        }

        Stack::Stack(SizeT size) : memory(static_cast<Byte *>(dvm_malloc(size))) {
            sp = memory + size;
        }

        void Stack::new_frame() {
            auto *addr = static_cast<Byte *>(dvm_malloc(sizeof(Frame)));
            auto *frame = new(addr) Frame(sp);
            frames.push_back(frame);
        }

        void Stack::remove_top_frame() {
            Frame *frame = current_frame();
            dvm_free(frame);
        }

        Byte *Frame::allocate(SizeT size) {
            // TODO: Check frame size
            sp -= size + sizeof(SizeT);
            *reinterpret_cast<SizeT *>(sp) = size;
            return sp + sizeof(SizeT);
        }

        Frame::Frame(Byte *bp) : bp(bp), sp(bp) {
        }

        Frame::~Frame() = default;
    }
}
