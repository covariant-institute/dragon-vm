//
// Created by kiva on 2017/9/7.
//
#pragma once

#include <core/dcx/dcx_file.hpp>
#include <string>
#include <cstdio>

namespace dvm {
    namespace core {
        namespace dcx {
            class DcxReader;

            class DcxReader {
            private:
                FILE *dcx_file;
                dcx_file_header dcx_header;

                bool read_header(dcx_file_header &header);

            public:
                DcxReader(const DcxReader &) = delete;

                DcxReader() = default;

                explicit DcxReader(const std::string &file_path);

                ~DcxReader();

                bool open(const std::string &file_path);

                void close();

                bool read_next_constant_entry(dcx_file_constant_entry &entry);

                bool read_next_class_entry(dcx_file_class_entry &entry);

                bool read_next_method_entry(dcx_file_method_entry &entry);
            };
        }
    }
}
