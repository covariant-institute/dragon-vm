//
// Created by kiva on 2017/10/10.
//

#pragma once

#include <core/type.hpp>
#include <core/object/slot.hpp>
#include <core/object/class.hpp>

namespace dvm {
    namespace core {
        namespace object {
            struct Array {
                SizeT length;
                TypeIdentifier array_type;
                Class *element_type;
                Slot elements[0];

                static Array *new_array(SizeT length, TypeIdentifier array_type);

                static Array *new_array(SizeT length);
            };
        }
    }
}
