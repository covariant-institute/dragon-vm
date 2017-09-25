//
// Created by kiva on 2017/9/24.
//
#pragma once

#include <core/type.hpp>
#include <string>
#include <unordered_map>

namespace dvm {
    namespace core {
        namespace object {
            class Class;

            class Method;
        }

        namespace runtime {
            class DragonVM;

            class VMContext;

            class Thread;

            class DragonVM {
                friend class VMContext;

            private:
                VMContext *bootstrapContext;
                Thread *bootstrapThread;

                std::unordered_map<std::string, object::Class *> class_map;
                std::unordered_map<std::string, std::unordered_map<std::string, object::Method *>> method_map;
                std::unordered_map<UInt16, std::string> constant_string_map;
                std::unordered_map<UInt16, object::Class *> constant_class_map;

                void register_class(const std::string &class_name, object::Class *prototype);

                object::Class *find_class(const std::string &class_name) const;

                void register_method(const std::string &method_name, const std::string &signature,
                                     object::Method *method);

                object::Method *resolve_method(const std::string &method_name,
                                               const std::string &signature) const;

                void register_constant(UInt16 constant_id, const std::string &data);

                void register_constant(UInt16 constant_id, object::Class *constant);

                const std::string &find_constant(UInt16 constant_id) const;

                const object::Class *find_class_constant(UInt16 constant_id);

            public:
                DragonVM();

                ~DragonVM();

                Thread *current_thread();

                void attachCurrentThread();

                void detachCurrentThread();
            };
        }
    }
}
