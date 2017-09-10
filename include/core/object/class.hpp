//
// Created by kiva on 2017/8/31.
//
#pragma once

#include <core/type.hpp>
#include <core/object/slot.hpp>
#include <string>

namespace dvm {
    namespace core {
        namespace object {
            struct Object;

            struct Class final {
                type_identifier type;
                Int32 class_slot_count;
                Int32 member_slot_count;

                Class *parent;
                std::string *name;

                /* Keep in last */
                Slot slots[0];

                static const Class* find_class(const std::string &name);

                static const Class* define_class(Class *parent, const std::string &name,
                                                 Int32 class_slot_count, Int32 member_slot_count);

                static Class* define_bootstrap_class(const std::string &name,
                                                     Int32 class_slot_count, Int32 member_slot_count);

                SizeT calculate_needed_size() const;

                Object *new_instance() const;

                Object *new_instance(Object *uninitialized) const;
            };
        }
    }
}
