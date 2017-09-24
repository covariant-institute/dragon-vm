//
// Created by kiva on 2017/9/24.
//
#pragma once

#include <core/type.hpp>

namespace dvm {
    namespace core {
        namespace runtime {
            struct JumpConditionEq {
                static bool get_result(Int32 check) {
                    return check == 0;
                }
            };

            struct JumpConditionNe {
                static bool get_result(Int32 check) {
                    return check != 0;
                }
            };

            struct JumpConditionGt {
                static bool get_result(Int32 check) {
                    return check > 0;
                }
            };

            struct JumpConditionGe {
                static bool get_result(Int32 check) {
                    return check >= 0;
                }
            };

            struct JumpConditionLt {
                static bool get_result(Int32 check) {
                    return check < 0;
                }
            };

            struct JumpConditionLe {
                static bool get_result(Int32 check) {
                    return check <= 0;
                }
            };
        }
    }
}

