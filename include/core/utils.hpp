#pragma once

#define dvm_memory_barrier() \
    asm volatile("" ::: "memory")

namespace dvm {
    namespace core {
    }
}