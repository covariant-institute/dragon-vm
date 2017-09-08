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

            dcx_reader::dcx_reader(const std::string &file_path) {
                open(file_path);
            }

            dcx_reader::~dcx_reader() {
                close();
            }

            bool dcx_reader::open(const std::string &file_path) {
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

            void dcx_reader::close() {
                if (dcx_file != nullptr) {
                    fclose(dcx_file);
                }
            }

            bool dcx_reader::read_header(dcx_file_header &header) {
                if (dcx_file == nullptr) {
                    return false;
                }
                fseek(dcx_file, 0, SEEK_SET);
                header.version_id = read_i32(dcx_file);
                return true;
            }

            bool dcx_reader::read_next_constant_entry(dcx_file_constant_entry &entry) {
                return false;
            }

            bool dcx_reader::read_next_class_entry(dcx_file_class_entry &entry) {
                return false;
            }

            bool dcx_reader::read_next_method_entry(dcx_file_method_entry &entry) {
                return false;
            }
        }
    }
}
