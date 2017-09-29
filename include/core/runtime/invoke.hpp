//
// Created by kiva on 2017/9/25.
//

#include <core/object/method.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            class InvokeHelper {
            public:

                static inline object::Method *resolve_by_id(Thread *thread, UInt16 name_id, UInt16 signature_id) {
                    VMContext *context = thread->get_context();
                    const std::string &name = context->find_constant(name_id);
                    const std::string &signature = context->find_constant(signature_id);

                    return thread->get_context()->resolve_method(name, signature);
                }

                /**
                 * Prepare to call a method.
                 */
                static inline void before_invoke(Thread *thread, object::Method *method) {
                    if (method == nullptr || thread == nullptr) {
                        return;
                    }

                    UInt16 locals_size = method->get_locals_size();
                    UInt16 args_size = method->get_args_size();

                    // share args_size bytes with current frame for argument passing
                    Frame *frame = thread->get_stack().new_frame(locals_size, args_size);

                    // return address
                    frame->set_last_pc(thread->pc);
                    frame->set_method(method);
                }
            };
        }
    }
}
