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
            Method::Method(const Class *return_type, const std::string &name, const std::string &signature,
                           Bool is_static_method, Bool is_native_method)
                    : return_type(return_type), method_name(name), method_signature(signature),
                      is_static_method(is_static_method), is_native_method(is_native_method) {
            }

            Method *Method::resolve(runtime::VMContext *context, const std::string &name,
                                    const std::string &signature) {
                return context->resolve_method(name, signature);
            }

            void Method::register_method(runtime::VMContext *context, const Class *return_type,
                                         const std::string &name, const std::string &signature,
                                         const dcx::DcxFileMethodEntry &entry) {
                object::Method *method = nullptr;
                if (entry.header.method_is_native) {
                    auto native_method = new NativeMethod(return_type, name, signature,
                                                          entry.header.method_is_static, True);
                    method = native_method;
                } else {
                    auto dvm_method = new DvmMethod(return_type, name, signature,
                                                entry.header.method_is_static, False);
                    dvm_method->method_body = entry.method_body;
                    dvm_method->method_length = entry.header.method_body_size;

                    method = dvm_method;
                }

                for (int i = 0; i < entry.header.method_handlers_count; ++i) {
                    auto *handler = entry.handlers + i;
                    auto *exception_class = context->find_class_constant(handler->exception_class_name_id);
                    method->handler.handlers[exception_class] = handler->handler_offset;
                }

                method->args_size = entry.header.method_args_size;
                method->locals_size = entry.header.method_locals_size;

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
                printf("  Max locals:  %u\n", method->get_locals_size());

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
