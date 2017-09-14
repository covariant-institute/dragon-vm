//
// Created by kiva on 2017/9/15.
//
#pragma once

#include <string>

namespace dvm {
    namespace core {
        namespace dl {
            using DLHandler = void *;
            using DLSymbol = void *;

            class DLInterface {
            public:
                virtual DLHandler open(const std::string &file) = 0;

                virtual DLSymbol find_symbol(const std::string &symbol_name) = 0;

                virtual void close() = 0;

                static DLInterface* open_library(const std::string &file);
            };
        }
    }
}
