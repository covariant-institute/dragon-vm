//
// Created by kiva on 2017/9/15.
//

#pragma once

#ifdef DVM_PLATFORM_UNIX

#include <core/platform/unix/dl_unix.hpp>

#endif

namespace dvm {
    namespace core {
        namespace dl {
#ifdef DVM_PLATFORM_UNIX
            using DLInterface = UnixDLInterface;
#endif
        }
    }
}
