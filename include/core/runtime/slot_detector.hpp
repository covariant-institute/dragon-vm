//
// Created by kiva on 2017/10/4.
//

#pragma once

#include <core/object/class.hpp>
#include <core/object/object.hpp>

namespace dvm {
    namespace core {
        namespace runtime {

            template <typename T>
            struct SlotDetector {
                static inline constexpr bool has_slot() {
                    return false;
                }
            };

            template <>
            struct SlotDetector<const object::Class*> {
                static inline constexpr bool has_slot() {
                    return true;
                }
            };

            template <>
            struct SlotDetector<const object::Object*> {
                static inline constexpr bool has_slot() {
                    return true;
                }
            };
        }
    }
}