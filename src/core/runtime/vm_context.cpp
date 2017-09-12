//
// Created by kiva on 2017/9/6.
//

#include <core/runtime/vm_context.hpp>

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
                    throw dvm::core::exception(DVM_CLASS_NOT_FOUND);
                }
            }
        }
    }
}
