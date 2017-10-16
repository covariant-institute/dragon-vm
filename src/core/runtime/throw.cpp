//
// Created by kiva on 2017/10/2.
//

#include <core/runtime/throw.hpp>
#include "../object/dvm_method.hpp"

namespace dvm {
    namespace core {
        namespace runtime {

            Byte *ThrowHelper::find_handler(Thread *thread, object::Object *ex) {
                // jump to handler if there's a method in frame
                auto *frame = thread->get_stack().current_frame();
                auto *method = frame->get_method();

                if (method == nullptr || method->is_native()) {
                    return nullptr;
                }

                auto &ex_handlers = method->get_handlers().handlers;
                auto iter = ex_handlers.find(ex->get_prototype());

                if (iter == ex_handlers.end()) {
                    return nullptr;
                }

                auto *dvm_method = dynamic_cast<object::DvmMethod*>(method);
                if (dvm_method == nullptr) {
                    return nullptr;
                }

                // offset of the handler
                return dvm_method->get_body() + iter->second;
            }
        }
    }
}

