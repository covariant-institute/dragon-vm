//
// Created by kiva on 2017/8/31.
//
#pragma once

#include <core/type.hpp>
#include <core/object/slot.hpp>
#include <string>

namespace dvm {
    namespace core {
        namespace runtime {
            class VMContext;
        }

        namespace object {
            struct Object;

            struct Class final {
                TypeIdentifier type;
                UInt32 class_slot_count;
                UInt32 member_slot_count;

                const Class *parent;
                std::string *name;

                /* Keep it last */
                Slot slots[0];

                static Class *find_class(runtime::VMContext *context, const std::string &name);

                static Class *define_class(runtime::VMContext *context, const Class *parent,
                                           const std::string &name,
                                           UInt32 class_slot_count, UInt32 member_slot_count);

                static Class *define_bootstrap_class(runtime::VMContext *context, const std::string &name,
                                                     UInt32 class_slot_count, UInt32 member_slot_count);

                inline bool is_primitive() const {
                    return type != TypeIdentifier::TYPE_ID_OBJECT
                           && type != TypeIdentifier::TYPE_ID_UNSPECIFIC;
                }

                SizeT calculate_needed_size() const;

                Object *new_instance() const;

                Object *new_instance(Object *uninitialized) const;
            };
        }
    }
}
