//
// Created by kiva on 2017/9/7.
//
#include <core/dcx/dcx_reader.hpp>
#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>

#include "reader_impl.hpp"

namespace dvm {
    namespace core {
        namespace dcx {

            DcxReader::DcxReader(const std::string &file_path) {
                open(file_path);
            }

            DcxReader::~DcxReader() {
                close();
            }

            bool DcxReader::open(const std::string &file_path) {
                dcx_file = fopen(file_path.c_str(), "rb");
                if (dcx_file == nullptr) {
                    return false;
                }

                try {
                    if (!read_header(dcx_header)) {
                        throw dvm::core::exception(DVM_DCX_INVALID);
                    }

                    if (!config::validate_version_id(dcx_header.version_id)) {
                        throw dvm::core::exception(DVM_DCX_INVALID_VERSION_ID);
                    }

                } catch (const dvm::core::exception &what) {
                    close();
                    throw what;
                }
                return true;
            }

            void DcxReader::close() {
                if (dcx_file != nullptr) {
                    fclose(dcx_file);
                }
            }

            bool DcxReader::read_header(dcx_file_header &header) {
                if (dcx_file == nullptr) {
                    return false;
                }
                fseek(dcx_file, 0, SEEK_SET);
                header.version_id = ByteOrderedReader::read_typed_data<Int32>(dcx_file);
                return true;
            }

            bool DcxReader::read_next_constant_entry(dcx_file_constant_entry &entry) {
                return false;
            }

            bool DcxReader::read_next_class_entry(dcx_file_class_entry &entry) {
                return false;
            }

            bool DcxReader::read_next_method_entry(dcx_file_method_entry &entry) {
                return false;
            }
        }
    }
}
