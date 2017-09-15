//
// Created by kiva on 2017/9/15.
//
#pragma once

#ifdef DVM_PLATFORM_WINDOWS

#include <core/platform/dl_commom.hpp>

namespace dvm {
    namespace core {
        namespace dl {
            class WindowsDLInterface {
            private:
                DLHandler handler;

            public:
                WindowsDLInterface();

                WindowsDLInterface(const WindowsDLInterface &) = delete;

                explicit WindowsDLInterface(const std::string &file);

                ~WindowsDLInterface();

                void open(const std::string &file);

                DLSymbol find_symbol(const std::string &symbol_name) const;

                void close();
            };
        }
    }
}

#endif
