//
// Created by kiva on 2017/9/15.
//
#pragma once

#include <core/platform/dl_commom.hpp>

namespace dvm {
    namespace core {
        namespace dl {
            class UnixDLInterface {
            private:
                DLHandler handler;

            public:
                UnixDLInterface();

                UnixDLInterface(const UnixDLInterface &) = delete;

                explicit UnixDLInterface(const std::string &file);

                ~UnixDLInterface();

                void open(const std::string &file);

                DLSymbol find_symbol(const std::string &symbol_name) const;

                void close();
            };
        }
    }
}
