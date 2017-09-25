//
// Created by kiva on 2017/9/7.
//
#include <core/dcx/dcx_reader.hpp>
#include <core/dcx/byte_ordered_reader.hpp>
#include <core/exceptions.hpp>

#include <core/errorcodes.hpp>
#include <core/memory.hpp>

namespace dvm {
    namespace core {
        namespace dcx {

            DcxReader::DcxReader(const std::string &file_path) {
                open(file_path);
            }

            DcxReader::~DcxReader() {
                close();
            }

            void DcxReader::open(const std::string &file_path) {
                dcx_file = fopen(file_path.c_str(), "rb");
                if (dcx_file == nullptr) {
                    throw dvm::core::Exception(DVM_DCX_NOT_OPEN);
                }

                try {
                    fseek(dcx_file, 0, SEEK_SET);

                    if (!read_header(dcx_file_info)) {
                        throw dvm::core::Exception(DVM_DCX_INVALID);
                    }

                    if (!config::validate_version_id(dcx_file_info.file_header.version_id)) {
                        throw dvm::core::Exception(DVM_DCX_INVALID_VERSION_ID);
                    }

                    if (!read_dcx_file_info(dcx_file_info)) {
                        throw dvm::core::Exception(DVM_DCX_INVALID);
                    }

                } catch (const dvm::core::Exception &what) {
                    close();
                    throw what;
                }
            }

            void DcxReader::close() {
                if (dcx_file != nullptr) {
                    fclose(dcx_file);
                }
            }

            bool DcxReader::read_header(DcxFileInfo &info) {
                if (dcx_file == nullptr) {
                    throw dvm::core::Exception(DVM_DCX_NOT_OPEN);
                }

                return ByteOrderedReader::read<config::VersionID>(dcx_file, &info.file_header.version_id);;
            }


            bool DcxReader::read_dcx_file_info(DcxFileInfo &info) {
                if (dcx_file == nullptr) {
                    throw dvm::core::Exception(DVM_DCX_NOT_OPEN);
                }

                if (!ByteOrderedReader::read<DcxFileConstantPoolHeader>(dcx_file, &info.constant_pool_header)) {
                    return false;
                }

                if (!ByteOrderedReader::read<DcxFileClassPoolHeader>(dcx_file, &info.class_pool_header)) {
                    return false;
                }

                return ByteOrderedReader::read<DcxFileMethodPoolHeader>(dcx_file, &info.method_pool_header);
            }

            bool DcxReader::read_constant_entry(DcxFileConstantEntry &entry) {
                if (dcx_file == nullptr) {
                    throw dvm::core::Exception(DVM_DCX_NOT_OPEN);
                }

                if (ByteOrderedReader::read<DcxFileConstantEntryHeader>(dcx_file, &entry.header)) {
                    entry.constant_data = (Byte *) dvm_malloc(sizeof(Byte) * entry.header.constant_data_size + 1);

                    if (ByteOrderedReader::read_bytes(dcx_file, entry.constant_data,
                                                      entry.header.constant_data_size)) {
                        entry.constant_data[entry.header.constant_data_size] = '\0';
                        return true;
                    }
                }
                return false;
            }

            bool DcxReader::read_class_entry(DcxFileClassEntry &entry) {
                if (dcx_file == nullptr) {
                    throw dvm::core::Exception(DVM_DCX_NOT_OPEN);
                }

                return ByteOrderedReader::read<DcxFileClassEntryHeader>(dcx_file, &entry.header);
            }

            bool DcxReader::read_method_entry(DcxFileMethodEntry &entry) {
                if (dcx_file == nullptr) {
                    throw dvm::core::Exception(DVM_DCX_NOT_OPEN);
                }

                entry.method_body = nullptr;
                entry.handlers = nullptr;

                if (ByteOrderedReader::read<DcxFileMethodEntryHeader>(dcx_file, &entry.header)) {
                    // Read method handlers
                    if (entry.header.method_handlers_count > 0) {
                        entry.handlers = (DcxFileMethodEntryHandler *) dvm_malloc(
                                sizeof(DcxFileMethodEntryHandler) * entry.header.method_handlers_count);
                        for (int i = 0; i < entry.header.method_handlers_count; ++i) {
                            ByteOrderedReader::read_bytes(dcx_file,
                                                          reinterpret_cast<Byte *>(entry.handlers + i),
                                                          sizeof(DcxFileMethodEntryHandler));
                        }
                    }

                    // Read method body
                    entry.method_body = (Byte *) dvm_malloc(sizeof(Byte) * entry.header.method_body_size);

                    return ByteOrderedReader::read_bytes(dcx_file, entry.method_body,
                                                         entry.header.method_body_size);
                }
                return false;
            }
        }
    }
}
