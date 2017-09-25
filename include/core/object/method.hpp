//
// Created by kiva on 2017/9/4.
//
#pragma once

#include <core/object/object.hpp>
#include <core/runtime/vm_context.hpp>
#include <core/dcx/dcx_file_info.hpp>

namespace dvm {
    namespace core {
        namespace object {

            class ExceptionHandler {
            public:
                // ExceptionType -> Handler's code_base offset
                std::unordered_map<const Class *, UInt16> handlers;

                inline SizeT get_handler_count() const {
                    return handlers.size();
                }
            };

            class Method {
            private:
                ExceptionHandler handler;

            public:
                inline const ExceptionHandler &get_handlers() const {
                    return handler;
                }

                virtual void invoke(runtime::Thread *thread) = 0;

                virtual Bool is_native() const = 0;

                virtual Bool is_static() const = 0;

                virtual const Class *get_return_type() const = 0;

                virtual const std::string &get_signature() const = 0;

                virtual const std::string &get_name() const = 0;

                static Method *resolve(runtime::VMContext *context,
                                       const std::string &name, const std::string &signature);

                static void register_method(runtime::VMContext *context, const Class *return_type,
                                            const std::string &name, const std::string &signature,
                                            Bool is_static_method, Byte *body, SizeT length,
                                            dcx::DcxFileMethodEntryHandler *handlers, SizeT handler_count);

                static void register_native_method(runtime::VMContext *context, const Class *return_type,
                                                   const std::string &name, const std::string &signature,
                                                   Bool is_static_method);

                static void dump_method_info(const Method *method);
            };
        }
    }
}