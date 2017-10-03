//
// Created by kiva on 2017/10/3.
//

#include <abi/resolve.hpp>
#include <sstream>

namespace dvm {
    namespace abi {
        std::string native_method_lookup_name(const std::string &name,
                                                            const std::string &signature) {
            std::stringstream buffer;
            buffer << "Dragon_";
            buffer << name;
            buffer << '_';
            buffer << signature;
            return buffer.str();
        }
    }
}
