//
// Created by kiva on 2017/9/4.
//
#pragma once

#include <core/object/object.hpp>
#include <core/runtime/vm_context.hpp>

namespace dvm {
    namespace core {
        namespace object {
            using FFIFunction = void (*)(runtime::VMContext &);

            class Method {
            public:
                virtual void invoke(runtime::VMContext &context) = 0;

                virtual Bool is_foreign_method() = 0;

                virtual const std::string &get_signature() = 0;

                virtual const std::string &get_name() = 0;

                static Method *resolve(const std::string &name, const std::string &signature);

                static Method *new_foreign_method(const std::string &name, const std::string &signature,
                                                  FFIFunction ffi);

                static Method *new_dvm_method(const std::string &name, const std::string &signature,
                                              Byte *body, SizeT length);
            };
        }
    }
}
