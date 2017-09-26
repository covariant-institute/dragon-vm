//
// Created by kiva on 2017/9/26.
//
#pragma once

#include <core/object/method.hpp>
#include <stack>

namespace dvm {
    namespace core {
        namespace runtime {
            struct CallStackEntry {
                object::Method *method;

                CallStackEntry(object::Method *method) : method(method) {
                }

                ~CallStackEntry() = default;
            };

            using CallStack = std::stack<CallStackEntry>;
        }
    }
}
