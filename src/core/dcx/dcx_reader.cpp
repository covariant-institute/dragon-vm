//
// Created by kiva on 2017/9/7.
//
#include <core/dcx/dcx_reader.hpp>
#include <core/dcx/byte_ordered_reader.hpp>
#include <core/exceptions.hpp>

#include <core/errorcodes.hpp>

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
                    fseek(dcx_file, 0, SEEK_SET);

                    if (!read_header(dcx_file_info)) {
                        throw dvm::core::exception(DVM_DCX_INVALID);
                    }

                    if (!config::validate_version_id(dcx_file_info.file_header.version_id)) {
                        throw dvm::core::exception(DVM_DCX_INVALID_VERSION_ID);
                    }

                    if (!read_dcx_file_info(dcx_file_info)) {
                        throw dvm::core::exception(DVM_DCX_INVALID);
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

            bool DcxReader::read_header(DcxFileInfo &info) {
                if (dcx_file == nullptr) {
                    return false;
                }

                return ByteOrderedReader::read<config::VersionID>(dcx_file, &info.file_header.version_id);;
            }


            bool DcxReader::read_dcx_file_info(DcxFileInfo &info) {
                if (dcx_file == nullptr) {
                    return false;
                }

                if (!ByteOrderedReader::read<DcxFileJumpTable>(dcx_file, &info.file_header.jump_table)) {
                    return false;
                }

                if (fseek(dcx_file, info.file_header.jump_table.constant_pool_start, SEEK_SET) != 0) {
                    return false;
                }

                if (!ByteOrderedReader::read<DcxFileConstantPoolHeader>(dcx_file, &info.constant_pool_header)) {
                    return false;
                }

                if (fseek(dcx_file, info.file_header.jump_table.class_pool_start, SEEK_SET) != 0) {
                    return false;
                }

                if (!ByteOrderedReader::read<DcxFileClassPoolHeader>(dcx_file, &info.class_pool_header)) {
                    return false;
                }

                if (fseek(dcx_file, info.file_header.jump_table.method_pool_start, SEEK_SET) != 0) {
                    return false;
                }

                return ByteOrderedReader::read<DcxFileMethodPoolHeader>(dcx_file, &info.method_pool_header);
            }

            bool DcxReader::read_next_constant_entry(DcxFileConstantEntry &entry) {
                return false;
            }

            bool DcxReader::read_next_class_entry(DcxFileClassEntry &entry) {
                return false;
            }

            bool DcxReader::read_next_method_entry(DcxFileMethodEntry &entry) {
                return false;
            }
        }
    }
}
