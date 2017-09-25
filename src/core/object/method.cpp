//
// Created by kiva on 2017/9/6.
//

#include <core/object/method.hpp>
#include <algorithm>
#include "dvm_method.hpp"
#include "native_method.hpp"

namespace dvm {
    namespace core {
        namespace object {
            Method *Method::resolve(runtime::VMContext *context, const std::string &name,
                                    const std::string &signature) {
                return context->resolve_method(name, signature);
            }

            void Method::register_method(runtime::VMContext *context, const Class *return_type,
                                         const std::string &name, const std::string &signature,
                                         Bool is_static_method, Byte *body, SizeT length,
                                         dcx::DcxFileMethodEntryHandler *handlers, SizeT handler_count) {

                auto method = new DvmMethod(return_type, name, signature, is_static_method, body, length);

                for (int i = 0; i < handler_count; ++i) {
                    auto *handler = handlers + i;
                    auto *exception_class = context->find_class_constant(handler->exception_class_name_id);
                    method->handler.handlers[exception_class] = handler->handler_offset;
                }

                context->register_method(name, signature, method);
            }

            void
            Method::register_native_method(runtime::VMContext *context, const Class *return_type,
                                           const std::string &name,
                                           const std::string &signature, Bool is_static_method) {

                auto method = new NativeMethod(return_type, name, signature, is_static_method);
                context->register_method(name, signature, method);
            }

            void Method::dump_method_info(const Method *method) {
                if (method == nullptr) {
                    return;
                }

                printf("Method <%s>\n", method->get_name().c_str());
                printf("  Return type: %s\n", method->get_return_type()->name->c_str());
                printf("  Native:      %s\n", method->is_native() ? "true" : "false");
                printf("  Static:      %s\n", method->is_static() ? "true" : "false");

                auto &handlers = method->get_handlers();
                if (handlers.get_handler_count() > 0) {
                    printf("  Handlers:\n");
                    int index = 0;
                    std::for_each(handlers.handlers.begin(), handlers.handlers.end(),
                                  [&index](const std::pair<const Class *, UInt16> &element) {
                                      printf("    #%d -> exception: %s, offset: %u\n",
                                             index++, element.first->name->c_str(), element.second);
                                  });
                }
            }
        }
    }
}
