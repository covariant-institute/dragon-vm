//
// Created by kiva on 2017/9/15.
//

#ifdef DVM_PLATFORM_WINDOWS

#include <core/platform/windows/dl_windows.hpp>
#include <windows.h>
#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>

namespace dvm {
    namespace core {
        namespace dl {

            WindowsDLInterface::WindowsDLInterface() : handler(nullptr) {

            }

            WindowsDLInterface::WindowsDLInterface(const std::string &file) : handler(nullptr) {
                open(file);
            }

            WindowsDLInterface::~WindowsDLInterface() {
                close();
            }

            void WindowsDLInterface::open(const std::string &file) {
                DLHandler handler = (DLHandler) ::LoadLibrary(file.c_str());
                if (handler == nullptr) {
                    throw dvm::core::Exception(DVM_DL_NOT_OPEN);
                }
                this->handler = handler;
            }

            DLSymbol WindowsDLInterface::find_symbol(const std::string &symbol_name) const {
                DLSymbol sym = nullptr;

                sym = ::GetProcAddress(handler != nullptr ? handler : GetModuleHandle(nullptr), symbol_name.c_str());
                if (sym != nullptr) {
                    return sym;
                }

                throw dvm::core::Exception(DVM_DL_NOT_FOUND);
            }

            void WindowsDLInterface::close() {
                if (handler != nullptr) {
                    ::FreeLibrary(handler);
                    handler = nullptr;
                }
            }
        }
    }
}

#endif
