//
// Created by kiva on 2017/9/10.
//

#include <core/stack.hpp>
#include <core/object/object.hpp>
#include <core/object/method.hpp>

namespace dvm {
    namespace core {

        Stack::~Stack() {
            frames.clear();
            dvm_free(memory);
        }

        Stack::Stack(SizeT size) : memory(static_cast<Byte *>(dvm_malloc(size))) {
            sp = memory + size;
        }

        Frame *Stack::new_frame(SizeT size, SizeT shared) {
            auto *bytes = static_cast<Byte *>(dvm_malloc(sizeof(Frame)));
            memset(bytes, '\0', sizeof(Frame));

            auto *frame = new(bytes) Frame(sp, size, shared);
            frames.push_back(frame);

            // Move sp forward
            sp = frame->frame_limit;
            return frame;
        }

        void Stack::remove_top_frame() {
            Frame *frame = current_frame();

            // Move sp backward
            sp = frame->base_pointer_no_shared;

            dvm_free(frame);
            frames.erase(frames.end() - 1);
        }

        Byte *Frame::allocate(SizeT size) {
            if (sp - size < frame_limit) {
                throw dvm::core::Exception(DVM_MEMORY_STACK_OVERFLOW);
            }
            sp -= size;
            return sp;
        }

        Frame::Frame(Byte *bp, SizeT size, SizeT shared)
                : frame_size(size), shared_size(shared),
                  base_pointer(bp + shared),
                  base_pointer_no_shared(bp),
                  sp(bp),
                  frame_limit(bp - size) {
        }

        Frame::~Frame() = default;
    }
}
