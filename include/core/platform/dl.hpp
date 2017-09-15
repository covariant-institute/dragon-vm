//
// Created by kiva on 2017/9/15.
//

#pragma once

#ifdef DVM_PLATFORM_UNIX

#include <core/platform/unix/dl_unix.hpp>

#elif DVM_PLATFORM_WINDOWS

#include <core/platform/windows/dl_windows.hpp>

#endif

namespace dvm {
    namespace core {
        namespace dl {
#ifdef DVM_PLATFORM_UNIX
            using DLInterface = UnixDLInterface;
#elif DVM_PLATFORM_WINDOWS
            using DLInterface = WindowsDLInterface;
#endif
        }
    }
}
