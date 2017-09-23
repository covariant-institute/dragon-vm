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
            template <typename LhsType, typename RhsType = LhsType>
            struct MathAdd {
                static inline LhsType get_result(LhsType &&lhs, LhsType &&rhs) {
                    return lhs + rhs;
                }
            };

            template <typename LhsType, typename RhsType = LhsType>
            struct MathSub {
                static inline LhsType get_result(LhsType &&lhs, LhsType &&rhs) {
                    return lhs - rhs;
                }
            };

            template <typename LhsType, typename RhsType = LhsType>
            struct MathMul {
                static inline LhsType get_result(LhsType &&lhs, LhsType &&rhs) {
                    return lhs * rhs;
                }
            };

            template <typename LhsType, typename RhsType = LhsType>
            struct MathDiv {
                static inline LhsType get_result(LhsType &&lhs, LhsType &&rhs) {
                    if (rhs == 0) {
                        throw dvm::core::Exception(DVM_RUNTIME_DIVIDE_BY_ZERO);
                    }
                    return lhs / rhs;
                }
            };

            template <typename LhsType, typename RhsType = LhsType>
            struct MathRemain {
                static inline LhsType get_result(LhsType &&lhs, LhsType &&rhs) {
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

            template <typename LhsType>
            struct MathNegate {
                static inline LhsType get_result(LhsType &&operand) {
                    return -operand;
                }
            };

            template <typename LhsType, typename RhsType = Int32>
            struct MathShiftLeft {
                static inline LhsType get_result(LhsType &&lhs, RhsType &&rhs) {
                    return lhs << rhs;
                }
            };

            template <typename LhsType, typename RhsType = Int32>
            struct MathShiftRight {
                static inline LhsType get_result(LhsType &&lhs, RhsType &&rhs) {
                    return lhs >> rhs;
                }
            };

            template <typename LhsType, typename RhsType = Int32>
            struct MathShiftLeftUnsigned;

            template <typename LhsType, typename RhsType = Int32>
            struct MathShiftRightUnsigned;

            template <>
            struct MathShiftLeftUnsigned<Int32, Int32> {
                static inline Int32 get_result(Int32 &&lhs, Int32 &&rhs) {
                    return static_cast<UInt32>(lhs) << rhs;
                }
            };


            template <>
            struct MathShiftLeftUnsigned<Int64, Int32> {
                static inline Int64 get_result(Int64 &&lhs, Int32 &&rhs) {
                    return static_cast<Int64>(lhs) << rhs;
                }
            };

            template <>
            struct MathShiftRightUnsigned<Int32, Int32> {
                static inline Int32 get_result(Int32 &&lhs, Int32 &&rhs) {
                    return static_cast<UInt32>(lhs) >> rhs;
                }
            };


            template <>
            struct MathShiftRightUnsigned<Int64, Int32> {
                static inline Int64 get_result(Int64 &&lhs, Int32 &&rhs) {
                    return static_cast<Int64>(lhs) >> rhs;
                }
            };

            template <typename LhsType, typename RhsType = LhsType>
            struct MathAnd {
                static inline LhsType get_result(LhsType &&lhs, RhsType &&rhs) {
                    return lhs & rhs;
                }
            };

            template <typename LhsType, typename RhsType = LhsType>
            struct MathOr {
                static inline LhsType get_result(LhsType &&lhs, RhsType &&rhs) {
                    return lhs | rhs;
                }
            };

            template <typename LhsType, typename RhsType = LhsType>
            struct MathXor {
                static inline LhsType get_result(LhsType &&lhs, RhsType &&rhs) {
                    return lhs ^ rhs;
                }
            };
        }
    }
}
