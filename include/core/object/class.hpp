//
// Created by kiva on 2017/8/31.
//
#pragma once

#include <core/type.hpp>
#include <string>

namespace dvm {
    namespace core {
        namespace object {
            struct Object;

            struct Class final {
                type_identifier type;
                int slot_count;
                Class *parent;
                std::string *name;

                static const Class* find_class(const std::string &name);

                static const Class* define_class(Class *parent, const std::string &name, int slot_count);

                static Class* define_bootstrap_class(const std::string &name, int slot_count);

                Object *create_object() const;
            };
        }
    }
}
