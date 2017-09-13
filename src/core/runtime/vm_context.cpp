//
// Created by kiva on 2017/9/6.
//

#include <core/runtime/vm_context.hpp>
#include <core/object/method.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            VMContext::VMContext(){
            }


            VMContext::~VMContext() {

            }

            void VMContext::register_class(const std::string &class_name, object::Class *prototype) {
                class_map[class_name] = prototype;
            }

            object::Class *VMContext::find_class(const std::string &class_name) {
                try {
                    return class_map.at(class_name);

                } catch (const std::out_of_range &e) {
                    throw dvm::core::exception(DVM_RUNTIME_CLASS_NOT_FOUND);
                }
            }

            void VMContext::register_method(const std::string &method_name, const std::string &signature,
                                            object::Method *method) {
                method_map[method_name][signature] = method;
            }

            object::Method *VMContext::resolve_method(const std::string &method_name,
                                                      const std::string &signature) {
                try {
                    return method_map.at(method_name).at(signature);

                } catch (const std::out_of_range &e) {
                    throw dvm::core::exception(DVM_RUNTIME_METHOD_NOT_FOUND);
                }
            }
        }
    }
}
