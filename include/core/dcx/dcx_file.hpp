//
// Created by kiva on 2017/9/11.
//

#pragma once

#include <core/dcx/dcx_file_info.hpp>
#include <memory>
#include <vector>
#include <string>

namespace dvm {
    namespace core {
        namespace dcx {
            class DcxFile;
            class DcxLinker;

            class DcxFile {
                friend class DcxLinker;

            private:
                std::vector<DcxFileConstantEntry> constant_pool;
                std::vector<DcxFileClassEntry> class_pool;
                std::vector<DcxFileMethodEntry> method_pool;

                DcxFile() = default;

                void load_dcx(const std::string &path);

                void load_dcx(Byte *bytes, SizeT length);

            public:
                ~DcxFile() = default;

                static std::shared_ptr<DcxFile> open(const std::string &path);
            };
        }
    }
}
