//
// Created by kiva on 2017/9/25.
//

#include <core/object/method.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            class InvokeHelper {
                using namespace object;

            public:
                /**
                 * Prepare to call a method.
                 */
                static inline void before_invoke(Thread *thread, Method *method) {
                    if (method == nullptr || thread == nullptr) {
                        return;
                    }

                    // allocate stack, we need one more sizeof(VMReturnAddr) bytes
                    // to store return address
                    UInt16 frame_size = method->get_frame_size() + sizeof(VMReturnAddr);
                    thread->get_stack().new_frame(frame_size);
                }

                static inline void after_invoke(Thread *thread, Method *method) {
                    if (method == nullptr || thread == nullptr) {
                        return;
                    }

                    thread->get_stack().remove_top_frame();
                }
            };
        }
    }
}
