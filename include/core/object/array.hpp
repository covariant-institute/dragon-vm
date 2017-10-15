//
// Created by kiva on 2017/10/10.
//

#pragma once

#include <core/type.hpp>
#include <core/object/object.hpp>

namespace dvm {
    namespace core {
        namespace object {
            struct Array {
                SizeT length;
                Class *element_type;
                Object *elements[0];

                static Array *new_array(Class *element_type, SizeT length);
            };
        }
    }
}
