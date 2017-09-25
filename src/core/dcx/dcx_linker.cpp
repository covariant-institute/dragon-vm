//
// Created by kiva on 2017/9/11.
//

#include <core/dcx/dcx_linker.hpp>
#include <core/object/method.hpp>
#include <algorithm>

namespace dvm {
    namespace core {
        namespace dcx {
            std::string DcxLinker::constant_to_string(const DcxFileConstantEntry &entry) {
                if (entry.constant_data == nullptr) {
                    throw dvm::core::Exception(DVM_DCX_LINKING_INVALID_NAME);
                }
                return std::string(reinterpret_cast<const char *>(entry.constant_data));
            }

            void DcxLinker::validate_class(DcxFileClassEntry &entry) {
                if (entry.header.member_slot_count < 1) {
                    throw dvm::core::Exception(DVM_DCX_LINKING_INVALID_CLASS);
                }
            }

            void DcxLinker::validate_method(DcxFileMethodEntry &entry) {
                if (!entry.header.method_is_native &&
                    (entry.header.method_body_size == 0 || entry.method_body == nullptr)) {
                    throw dvm::core::Exception(DVM_DCX_LINKING_INVALID_METHOD);
                }

                if (!entry.header.method_is_native
                    && entry.header.method_handlers_count > 0
                    && entry.handlers == nullptr) {
                    throw dvm::core::Exception(DVM_DCX_LINKING_INVALID_METHOD);
                }

                if (entry.header.method_is_native
                    && entry.header.method_handlers_count > 0) {
                    throw dvm::core::Exception(DVM_DCX_LINKING_INVALID_METHOD);
                }
            }

            void DcxLinker::link(runtime::VMContext *context, std::shared_ptr<DcxFile> dcx_file) {
                std::vector<UInt16> link_after_class{ };

                link_constant(context, dcx_file, link_after_class);

                link_class(context, dcx_file);
                std::for_each(link_after_class.begin(), link_after_class.end(),
                              [&](UInt16 class_name_id) {
                                  auto constant = context->find_class(context->find_constant(class_name_id));
                                  context->register_constant(class_name_id, constant);
                              });

                link_method(context, dcx_file);
            }

            void DcxLinker::link_class(runtime::VMContext *context, std::shared_ptr<DcxFile> dcx_file) {
                std::for_each(dcx_file->class_pool.begin(), dcx_file->class_pool.end(),
                              [&](DcxFileClassEntry &entry) {
                                  using namespace dvm::core::object;

                                  validate_class(entry);

                                  std::string parent_class_name = context->find_constant(
                                          entry.header.parent_class_name_id);
                                  std::string class_name = context->find_constant(entry.header.class_name_id);

                                  auto *parent = context->find_class(parent_class_name);
                                  Class::define_class(context, parent,
                                                      class_name,
                                                      entry.header.class_slot_count, entry.header.member_slot_count);
                              });
            }

            void DcxLinker::link_method(runtime::VMContext *context, std::shared_ptr<DcxFile> dcx_file) {
                std::for_each(dcx_file->method_pool.begin(), dcx_file->method_pool.end(),
                              [&](DcxFileMethodEntry &entry) {
                                  using namespace dvm::core::object;

                                  validate_method(entry);
                                  std::string name = context->find_constant(entry.header.method_name_id);
                                  std::string signature = context->find_constant(entry.header.method_signature_id);

                                  auto *return_type_class = context->find_class_constant(
                                          entry.header.method_return_type_name_id);

                                  if (entry.header.method_is_native) {
                                      Method::register_native_method(context, return_type_class,
                                                                     name, signature, entry.header.method_is_static);
                                      return;
                                  }

                                  Method::register_method(context, return_type_class, name, signature,
                                                          entry.header.method_is_static, entry.method_body,
                                                          entry.header.method_body_size, entry.handlers,
                                                          entry.header.method_handlers_count);
                              });
            }

            void DcxLinker::link_constant(runtime::VMContext *context, std::shared_ptr<DcxFile> dcx_file,
                                          std::vector<UInt16> &class_constants) {
                std::for_each(dcx_file->constant_pool.begin(), dcx_file->constant_pool.end(),
                              [&](DcxFileConstantEntry &entry) {
                                  context->register_constant(entry.header.constant_id,
                                                             constant_to_string(entry));
                                  if (entry.header.constant_type == CONSTANT_CLASS) {
                                      class_constants.push_back(entry.header.constant_id);
                                  }
                              });
            }
        }
    }
}
