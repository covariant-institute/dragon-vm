//
// Created by kiva on 2017/9/23.
//

#pragma once

#include <core/type.hpp>
#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>
#include <cmath>

namespace dvm {
    namespace core {
        namespace runtime {
            template <typename T>
            struct MathAdd {
                static inline T get_result(T &&lhs, T &&rhs) {
                    return lhs + rhs;
                }
            };

            template <typename T>
            struct MathSub {
                static inline T get_result(T &&lhs, T &&rhs) {
                    return lhs - rhs;
                }
            };

            template <typename T>
            struct MathMul {
                static inline T get_result(T &&lhs, T &&rhs) {
                    return lhs * rhs;
                }
            };

            template <typename T>
            struct MathDiv {
                static inline T get_result(T &&lhs, T &&rhs) {
                    if (rhs == 0) {
                        throw dvm::core::Exception(DVM_RUNTIME_DIVIDE_BY_ZERO);
                    }
                    return lhs / rhs;
                }
            };

            template <typename T>
            struct MathRemain {
                static inline T get_result(T &&lhs, T &&rhs) {
                    return lhs % rhs;
                }
            };

            template <>
            struct MathRemain<Float> {
                static inline Float get_result(Float &&lhs, Float &&rhs) {
                    return fmodf(lhs, rhs);
                }
            };

            template <>
            struct MathRemain<Double> {
                static inline Double get_result(Double &&lhs, Double &&rhs) {
                    return fmod(lhs, rhs);
                }
            };

            template <typename T>
            struct MathNegate {
                static inline T get_result(T &&operand) {
                    return -operand;
                }
            };
        }
    }
}
