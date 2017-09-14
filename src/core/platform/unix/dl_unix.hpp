//
// Created by kiva on 2017/9/15.
//
#pragma once

#include <core/platform/dl.hpp>

namespace dvm {
    namespace core {
        namespace dl {
            class UnixDLInterface : public DLInterface {
                DLHandler open(const std::string &file) override;

                DLSymbol find_symbol(const std::string &symbol_name) override;

                void close() override;
            };
        }
    }
}
