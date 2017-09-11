//
// Created by kiva on 2017/9/11.
//

#include <core/dcx/dcx_linker.hpp>
#include <algorithm>

namespace dvm {
    namespace core {
        namespace dcx {
            void DcxLinker::validate(runtime::VMContext &context, std::shared_ptr<DcxFile> dcx_file) {

            }

            void DcxLinker::link(runtime::VMContext &context, std::shared_ptr<DcxFile> dcx_file) {
                std::for_each(dcx_file->class_pool.begin(), dcx_file->class_pool.end(),
                              [&](dcx::DcxFileClassEntry &entry) {

                              });
            }
        }
    }
}
