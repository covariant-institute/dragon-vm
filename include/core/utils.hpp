#pragma once

#define dvm_memory_barrier() \
    asm volatile("" ::: "memory")

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