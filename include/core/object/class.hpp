//
// Created by kiva on 2017/8/31.
//
#pragma once

#include <core/type.hpp>
#include <string>

namespace dvm {
    namespace core {
        namespace object {
            struct Class {
                type_identifier type;
                std::string name;
                Class *parent;

                static Class* find_class(const std::string &name);
            };
        }
    }
}
