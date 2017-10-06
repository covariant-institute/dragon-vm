//
// Created by kiva on 2017/9/4.
//
#pragma once

#include <core/object/object.hpp>
#include <core/runtime/context.hpp>
#include <core/dcx/dcx_file_info.hpp>

namespace dvm {
    namespace core {
        namespace object {

            struct MethodRegistry {
                std::unordered_map<const Class *, UInt16> handlers;

                UInt16 locals_size;
                UInt16 args_size;
                UInt32 body_size;
                Bool is_native;
                Bool is_static;
                Byte *body;
            };

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
                std::string method_name;
                std::string method_signature;
                const Class *return_type;
                Bool is_native_method;
                Bool is_static_method;
                UInt16 locals_size;
                UInt16 args_size;

                // for optimizing, we count how many times a method has been invoked.
                UInt16 invoked_times;

            protected:
                /**
                 * Called when a method is being executed.
                 *
                 * @param thread Executing thread.
                 */
                virtual void prepare(runtime::Thread *thread) = 0;

            public:
                Method(const Class *return_type,
                       const std::string &name,
                       const std::string &signature,
                       Bool is_static_method,
                       Bool is_native_method);

                virtual ~Method() = default;

                virtual void dump() const;

                void invoke(runtime::Thread *thread);

                inline const ExceptionHandler &get_handlers() const {
                    return handler;
                }

                inline Bool is_native() const {
                    return is_native_method;
                }

                inline Bool is_static() const {
                    return is_static_method;
                }

                inline const Class *get_return_type() const {
                    return return_type;
                }

                inline const std::string &get_signature() const {
                    return method_signature;
                }

                inline const std::string &get_name() const {
                    return method_name;
                }

                inline UInt16 get_locals_size() const {
                    return locals_size;
                }

                inline UInt16 get_args_size() const {
                    return args_size;
                }

                static Method *resolve(runtime::VMContext *context,
                                       const std::string &name, const std::string &signature);

                static void register_method(runtime::VMContext *context, const Class *return_type,
                                            const std::string &name, const std::string &signature,
                                            const MethodRegistry &registry);

                static void dump_method_info(const Method *method);
            };
        }
    }
}