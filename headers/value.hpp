#pragma once

#include "type.hpp"

namespace dvm {
    constexpr ULong stack_default_size = static_cast<ULong>(10240);
    enum class type_code : UInt {
        _char = 1, _int = 2, _long = 3, _uchar = 4, _uint = 5, _ulong = 6, _float = 7, _double = 8
    };
}
