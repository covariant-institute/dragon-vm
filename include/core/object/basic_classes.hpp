//
// Created by kiva on 2017/9/1.
//

#pragma once

#include <core/object/object.hpp>

namespace dvm {
    namespace core {
        namespace object {
            void init_base_classes();

#define CREATOR_SIGNATURE(TYPE) \
            Object* new_##TYPE(TYPE value)

            CREATOR_SIGNATURE(Int8);

            CREATOR_SIGNATURE(Int16);

            CREATOR_SIGNATURE(Int32);

            CREATOR_SIGNATURE(Int64);

            CREATOR_SIGNATURE(UInt8);

            CREATOR_SIGNATURE(UInt16);

            CREATOR_SIGNATURE(UInt32);

            CREATOR_SIGNATURE(UInt64);

            CREATOR_SIGNATURE(Float);

            CREATOR_SIGNATURE(Double);

            CREATOR_SIGNATURE(Byte);

            CREATOR_SIGNATURE(Bool);

#undef CREATOR_SIGNATURE
        }
    }
}
