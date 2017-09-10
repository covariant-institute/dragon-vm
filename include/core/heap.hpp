#pragma once

#include <core/type.hpp>
#include <core/memory.hpp>
#include <core/stack.hpp>
#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <list>

namespace dvm {
    namespace core {
        class Heap final {
        public:
            enum class AllocationPolicy {
                FIRST_FIT, BEST_FIT, WORST_FIT
            };

        private:
            // heap start
            void *hs = nullptr;

            // heap pointer
            Byte *hp = nullptr;

            // heap limit
            Byte *hl = nullptr;

            AllocationPolicy policy = AllocationPolicy::BEST_FIT;
            std::list<Byte *> free_list;

            inline SizeT &get_size(Byte *ptr) {
                return *reinterpret_cast<SizeT *>(ptr);
            }

            inline void sort_by_size() {
                free_list.sort([this](Byte *lhs, Byte *rhs) {
                    return get_size(lhs) < get_size(rhs);
                });
            }

            inline void sort_by_size_reverse() {
                free_list.sort([this](Byte *lhs, Byte *rhs) {
                    return get_size(lhs) > get_size(rhs);
                });
            }

            inline void sort_by_address() {
                free_list.sort([this](Byte *lhs, Byte *rhs) {
                    return lhs < rhs;
                });
            }

            void compress();

            void *find_in_free_list(SizeT size);

        public:
            Heap() = delete;

            Heap(const Heap &) = delete;

            explicit Heap(SizeT size, AllocationPolicy p = AllocationPolicy::BEST_FIT);

            ~Heap();

            void *malloc(SizeT size);

            void free(void *ptr);

            inline SizeT size_of(void *ptr) {
                return get_size(reinterpret_cast<Byte *>(ptr));
            }
        };
    }
}