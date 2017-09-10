//
// Created by kiva on 2017/9/7.
//
#pragma once

#include <core/dcx/dcx_file_info.hpp>
#include <string>
#include <cstdio>

namespace dvm {
    namespace core {
        namespace dcx {
            class DcxReader;


            class DcxReader {
            private:
                FILE *dcx_file;
                DcxFileInfo dcx_file_info;

                bool read_header(DcxFileInfo &info);

                bool read_dcx_file_info(DcxFileInfo &info);

            public:

                DcxReader(const DcxReader &) = delete;

                DcxReader() = default;

                explicit DcxReader(const std::string &file_path);

                ~DcxReader();

                bool open(const std::string &file_path);

                void close();

                bool read_next_constant_entry(DcxFileConstantEntry &entry);

                bool read_next_class_entry(DcxFileClassEntry &entry);

                bool read_next_method_entry(DcxFileMethodEntry &entry);

                inline UInt32 get_constant_entries() const {
                    return dcx_file_info.constant_pool_header.constant_entries;
                }

                inline UInt32 get_class_entries() const {
                    return dcx_file_info.class_pool_header.class_entries;
                }

                inline UInt32 get_method_entries() const {
                    return dcx_file_info.method_pool_header.method_entries;
                }
            };
        }
    }
}
