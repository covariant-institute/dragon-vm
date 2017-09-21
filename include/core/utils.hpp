#pragma once

#define dvm_offset_of(TYPE, MEMBER) \
    ((size_t) &((TYPE *) nullptr)->MEMBER)

namespace dvm {
    namespace core {

        template <typename T, typename R>
        struct isSameType {
            static constexpr bool value = false;
        };

        template <typename T>
        struct isSameType<T, T> {
            static constexpr bool value = true;
        };
    }
}