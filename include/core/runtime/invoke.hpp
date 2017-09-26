//
// Created by kiva on 2017/9/25.
//

#include <core/object/method.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            class InvokeHelper {
            public:
                static inline VMReturnAddr pc_to_return_address(VMOpcode *pc) {
                    return reinterpret_cast<VMReturnAddr &&>(pc);
                }

                /**
                 * Prepare to call a method.
                 */
                static inline void before_invoke(Thread *thread, object::Method *method) {
                    if (method == nullptr || thread == nullptr) {
                        return;
                    }

                    // allocate stack, we need one more sizeof(VMReturnAddr) bytes
                    // to store return address
                    UInt16 frame_size = method->get_frame_size() + sizeof(VMReturnAddr);
                    thread->get_stack().new_frame(frame_size);

                    // store return address
                    thread->get_stack().push<VMReturnAddr>(pc_to_return_address(thread->pc));

                    // finally, store caller
                    thread->calling_stack.push(CallStackEntry(method));
                }

                static inline void after_invoke(Thread *thread, object::Method *method) {
                    if (method == nullptr || thread == nullptr) {
                        return;
                    }

                    // clean stack memory
                    thread->get_stack().remove_top_frame();

                    // remove caller
                    thread->calling_stack.pop();
                }
            };
        }
    }
}
