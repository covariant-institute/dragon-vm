//
// Created by kiva on 2017/9/15.
//

#ifdef DVM_PLATFORM_UNIX

// for RTLD_DEFAULT
#define _GNU_SOURCE

#include <core/platform/unix/dl_unix.hpp>
#include <dlfcn.h>
#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>

namespace dvm {
    namespace core {
        namespace dl {
            UnixDLInterface::UnixDLInterface() : handler(nullptr) {
            }

            UnixDLInterface::UnixDLInterface(const std::string &file) : handler(nullptr) {
                open(file);
            }

            UnixDLInterface::~UnixDLInterface() {
                close();
            }

            void UnixDLInterface::open(const std::string &file) {
                DLHandler handler = ::dlopen(file.c_str(), RTLD_LAZY);
                if (handler == nullptr) {
                    throw dvm::core::exception(DVM_DL_NOT_OPEN);
                }
                this->handler = handler;
            }

            DLSymbol UnixDLInterface::find_symbol(const std::string &symbol_name) const {
                DLSymbol sym = nullptr;

                // Clear errors
                (void) ::dlerror();

                sym = dlsym(handler != nullptr ? handler : RTLD_DEFAULT, symbol_name.c_str());
                const char *error = ::dlerror();

                if (error == nullptr) {
                    return sym;
                }

                throw dvm::core::exception(DVM_DL_NOT_FOUND);
            }

            void UnixDLInterface::close() {
                if (handler != nullptr) {
                    ::dlclose(handler);
                    handler = nullptr;
                }
            }
        }
    }
}

#endif