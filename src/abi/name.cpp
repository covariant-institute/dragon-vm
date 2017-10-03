//
// Created by kiva on 2017/10/3.
//
#include <abi/name.hpp>
#include <sstream>

namespace dvm {
    namespace abi {
        std::string name_mangle(const std::string &s) {
            std::stringstream buffer;
            std::string::size_type length = s.size();
            return buffer.str();
        }
    }
}

