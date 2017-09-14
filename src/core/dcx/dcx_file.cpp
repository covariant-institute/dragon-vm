//
// Created by kiva on 2017/9/11.
//

#include <core/dcx/dcx_file.hpp>
#include <core/dcx/dcx_reader.hpp>
#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>
#include <core/memory.hpp>

namespace dvm {
    namespace core {
        namespace dcx {

            DcxFile::~DcxFile() {
                std::for_each(method_pool.begin(), method_pool.end(),
                              [&](DcxFileMethodEntry &entry) {
                                  if (!entry.header.method_is_native) {
                                      dvm_free(entry.method_body);
                                  }
                              });

                std::for_each(constant_pool.begin(), constant_pool.end(),
                              [&](DcxFileConstantEntry &entry) {
                                  dvm_free(entry.constant_data);
                              });
            }

            void DcxFile::load_dcx(const std::string &path) {
                DcxReader reader(path);

                DcxFileConstantEntry constant_entry{ };
                for (int i = 0; i < reader.get_constant_entries(); ++i) {
                    if (!reader.read_constant_entry(constant_entry)) {
                        throw dvm::core::exception(DVM_DCX_LOAD_ERROR);
                    }
                    constant_pool.push_back(constant_entry);
                }

                DcxFileClassEntry class_entry{ };
                for (int i = 0; i < reader.get_class_entries(); ++i) {
                    if (!reader.read_class_entry(class_entry)) {
                        throw dvm::core::exception(DVM_DCX_LOAD_ERROR);
                    }
                    class_pool.push_back(class_entry);
                }

                DcxFileMethodEntry method_entry{ };
                for (int i = 0; i < reader.get_method_entries(); ++i) {
                    if (!reader.read_method_entry(method_entry)) {
                        throw dvm::core::exception(DVM_DCX_LOAD_ERROR);
                    }
                    method_pool.push_back(method_entry);
                }

                reader.close();
            }

            void DcxFile::load_dcx(Byte *bytes, SizeT length) {
                throw dvm::core::exception(DVM_DCX_LOAD_ERROR);
            }

            std::shared_ptr<DcxFile> DcxFile::open(const std::string &path) {
                std::shared_ptr<DcxFile> dcx(new DcxFile);
                dcx->load_dcx(path);
                return dcx;
            }
        }
    }
}
