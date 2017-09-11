//
// Created by kiva on 2017/9/11.
//

#pragma once

#include <core/dcx/dcx_file_info.hpp>
#include <memory>
#include <unordered_map>
#include <string>

namespace dvm {
    namespace core {
        namespace dcx {
            class DcxFile;

            class DcxFile {
            private:
                std::unordered_map<UInt32, DcxFileConstantEntry> constant_pool;
                std::unordered_map<UInt32, DcxFileClassEntry> class_pool;
                std::unordered_map<UInt32, DcxFileMethodEntry> method_pool;

                DcxFile() = default;

                void load_dcx(const std::string &path);

                void load_dcx(Byte *bytes, SizeT length);

            public:
                ~DcxFile() = default;

                DcxFileConstantEntry get_constant(UInt32 constant_id);

                static std::unique_ptr<DcxFile> open(const std::string &path);
            };
        }
    }
}
