//
// Created by kiva on 2017/9/4.
//
#pragma once

#include <core/object/object.hpp>
#include <core/runtime/vm_context.hpp>

namespace dvm {
    namespace core {
        namespace object {

            class Method {
            public:
                virtual void invoke(runtime::VMContext &context) = 0;

                virtual Bool is_native() = 0;

                virtual Bool is_static() = 0;

                virtual Class *get_return_type() = 0;

                virtual const std::string &get_signature() = 0;

                virtual const std::string &get_name() = 0;

                static Method *resolve(runtime::VMContext &context,
                                       const std::string &name, const std::string &signature);

                static void register_method(runtime::VMContext &context, Class *return_type,
                                            const std::string &name, const std::string &signature,
                                            Bool is_static_method, Byte *body, SizeT length);

                static void register_native_method(runtime::VMContext &context, Class *return_type,
                                            const std::string &name, const std::string &signature,
                                            Bool is_static_method);
            };
        }
    }
}