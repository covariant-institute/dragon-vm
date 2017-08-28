//
// Created by kiva on 2017/8/28.
//

#include <core/config.hpp>

namespace dvm {
    namespace core {
        namespace config {
            bool validate_version_id(Int version_id) {
                return version_id <= make_version_id();
            }
        }
    }
}
