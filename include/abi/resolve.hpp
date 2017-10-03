//
// Created by kiva on 2017/10/3.
//

#pragma once

#include <string>

namespace dvm {
    namespace abi {
        std::string native_method_lookup_name(const std::string &name,
                                                     const std::string &signature);
    }
}
