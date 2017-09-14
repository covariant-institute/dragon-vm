//
// Created by kiva on 2017/9/15.
//

#include "dl_unix.hpp"

namespace dvm {
    namespace core {
        namespace dl {


            DLHandler UnixDLInterface::open(const std::string &file) {
                return nullptr;
            }

            DLSymbol UnixDLInterface::find_symbol(const std::string &symbol_name) {
                return nullptr;
            }

            void UnixDLInterface::close() {

            }
        }
    }
}
