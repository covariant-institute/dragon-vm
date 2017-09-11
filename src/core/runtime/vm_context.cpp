//
// Created by kiva on 2017/9/6.
//

#include <core/runtime/vm_context.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            VMContext::VMContext()
                    : stack(config::STACK_DEFAULT_SIZE) {
            }

            VMContext::VMContext(std::shared_ptr<dcx::DcxFile> dcx_file)
                    : VMContext() {
                set_file(dcx_file);
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

            void VMContext::set_file(const std::shared_ptr<dcx::DcxFile> &dcx_file) {
                VMContext::dcx_file = dcx_file;
            }
        }
    }
}
