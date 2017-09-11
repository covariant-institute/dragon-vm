//
// Created by kiva on 2017/9/11.
//

#include <core/dcx/dcx_file.hpp>

namespace dvm {
    namespace core {
        namespace dcx {


            void DcxFile::load_dcx(const std::string &path) {

            }

            void DcxFile::load_dcx(Byte *bytes, SizeT length) {

            }

            std::unique_ptr<DcxFile> DcxFile::open(const std::string &path) {
                return std::unique_ptr<DcxFile>();
            }

            DcxFileConstantEntry DcxFile::get_constant(UInt32 constant_id) {
                return DcxFileConstantEntry();
            }
        }
    }
}
