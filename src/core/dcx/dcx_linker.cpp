//
// Created by kiva on 2017/9/11.
//

#include <core/dcx/dcx_linker.hpp>
#include <core/object/class.hpp>
#include <algorithm>

namespace dvm {
    namespace core {
        namespace dcx {
            void DcxLinker::validate(runtime::VMContext &context, std::shared_ptr<DcxFile> dcx_file) {

            }

            void DcxLinker::link(runtime::VMContext &context, std::shared_ptr<DcxFile> dcx_file) {
                std::for_each(dcx_file->class_pool.begin(), dcx_file->class_pool.end(),
                              [&](dcx::DcxFileClassEntry &entry) {
                                  using object::Class;

                                  auto parent_class_name_entry = dcx_file->get_constant(
                                          entry.header.parent_class_name_id);
                                  auto class_name_entry = dcx_file->get_constant(entry.header.class_name_id);
                                  if (parent_class_name_entry.constant_data == nullptr
                                      || class_name_entry.constant_data == nullptr) {
                                      throw dvm::core::exception(DVM_DCX_LINKING_INVALID_NAME);
                                  }

                                  std::string parent_class_name(reinterpret_cast<const char *>(parent_class_name_entry.constant_data));
                                  std::string class_name(reinterpret_cast<const char *>(class_name_entry.constant_data));

                                  auto *parent = Class::find_class(context, parent_class_name);
                                  Class::define_class(context, parent,
                                                      class_name,
                                                      entry.header.class_slot_count, entry.header.member_slot_count);
                              });
            }
        }
    }
}
