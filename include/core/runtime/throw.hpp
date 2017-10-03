//
// Created by kiva on 2017/10/2.
//

#pragma once

#include <core/runtime/thread.hpp>
#include <core/object/object.hpp>
#include <core/object/method.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            class ThrowHelper {
            public:
                static Byte *find_handler(Thread *thread, object::Object *ex);
            };
        }
    }
}

