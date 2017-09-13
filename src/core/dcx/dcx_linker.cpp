//
// Created by kiva on 2017/9/11.
//

#include <core/dcx/dcx_linker.hpp>
#include <core/object/method.hpp>

namespace dvm {
    namespace core {
        namespace dcx {
            std::string DcxLinker::constant_to_string(std::shared_ptr<DcxFile> dcx_file, UInt32 constant_id) {
                auto entry = dcx_file->get_constant(constant_id);
                if (entry.constant_data == nullptr) {
                    throw dvm::core::exception(DVM_DCX_LINKING_INVALID_NAME);
                }
                return std::string(reinterpret_cast<const char *>(entry.constant_data));
            }

            void DcxLinker::validate_class(DcxFileClassEntry &entry) {
                if (entry.header.member_slot_count < 1) {
                    throw dvm::core::exception(DVM_DCX_LINKING_INVALID_CLASS);
                }
            }

            void DcxLinker::validate_method(DcxFileMethodEntry &entry) {
                if (!entry.header.method_is_native &&
                    (entry.header.method_length == 0 || entry.method_body == nullptr)) {
                    throw dvm::core::exception(DVM_DCX_LINKING_INVALID_METHOD);
                }
            }

            void DcxLinker::link(runtime::VMContext &context, std::shared_ptr<DcxFile> dcx_file) {
                link_class(context, dcx_file);
                link_method(context, dcx_file);
            }

            void DcxLinker::link_class(runtime::VMContext &context, std::shared_ptr<DcxFile> dcx_file) {
                std::for_each(dcx_file->class_pool.begin(), dcx_file->class_pool.end(),
                              [&](dcx::DcxFileClassEntry &entry) {
                                  using namespace dvm::core::object;

                                  validate_class(entry);

                                  std::string parent_class_name = constant_to_string(
                                          dcx_file, entry.header.parent_class_name_id);
                                  std::string class_name = constant_to_string(
                                          dcx_file, entry.header.class_name_id);

                                  auto *parent = context.find_class(parent_class_name);
                                  Class::define_class(context, parent,
                                                      class_name,
                                                      entry.header.class_slot_count, entry.header.member_slot_count);
                              });
            }

            void DcxLinker::link_method(runtime::VMContext &context, std::shared_ptr<DcxFile> dcx_file) {
                std::for_each(dcx_file->method_pool.begin(), dcx_file->method_pool.end(),
                              [&](dcx::DcxFileMethodEntry &entry) {
                                  using namespace dvm::core::object;

                                  validate_method(entry);
                                  std::string name = constant_to_string(dcx_file, entry.header.method_name_id);
                                  std::string signature = constant_to_string(
                                          dcx_file, entry.header.method_signature_id);
                                  std::string return_type = constant_to_string(
                                          dcx_file, entry.header.method_return_type_name_id);

                                  auto *return_type_class = context.find_class(return_type);
                                  if (entry.header.method_is_native) {
                                      Method::register_native_method(context, return_type_class,
                                                                     name, signature, entry.header.method_is_static);
                                      return;
                                  }

                                  Method::register_method(context, return_type_class, name, signature,
                                                          entry.header.method_is_static, entry.method_body,
                                                          entry.header.method_length);
                              });
            }
        }
    }
}
