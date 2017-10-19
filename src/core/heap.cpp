//
// Created by Mike Lee on 2017/8/29.
//

#define DVM_DEBUG

#include <core/heap.hpp>

namespace dvm {
    namespace core {

        Heap::Heap(SizeT size, Heap::AllocationPolicy p) : hs(dvm_malloc(size)), policy(p) {
            hp = reinterpret_cast<Byte *>(hs);
            hl = hp + size;
        }

        Heap::~Heap() {
            dvm_free(hs);
        }

        void Heap::compress() {
            // Sort the spaces by address.
            sort_by_address();
            std::list<Byte *> new_list;
            Byte *ptr = nullptr;
            // Compress the free list.
            for (auto p : free_list) {
                if (ptr != nullptr) {
                    SizeT &size = get_size(ptr);
                    if (ptr + size + sizeof(SizeT) == p) {
                        size += get_size(p) + sizeof(SizeT);
                    } else {
                        new_list.push_back(ptr);
                        ptr = p;
                    }
                } else
                    ptr = p;
            }
            // Connect the final space and remain spaces.
            get_size(ptr) += hl - hp;
            hp = hl;
            new_list.push_back(ptr);
            // Swap the new list and old list.
            std::swap(new_list, free_list);
        }

        void *Heap::find_in_free_list(SizeT size) {
            if (!free_list.empty()) {
                switch (policy) {
                    case AllocationPolicy::FIRST_FIT:
                        break;
                    case AllocationPolicy::BEST_FIT:
                        sort_by_size();
                        break;
                    case AllocationPolicy::WORST_FIT:
                        sort_by_size_reverse();
                        break;
                }
                // Find the first fit space.
                auto it = free_list.begin();
                for (; it != free_list.end(); ++it)
                    if (get_size(*it) >= size)
                        break;
                if (it != free_list.end()) {
                    // Remove from free list.
                    void *ptr = reinterpret_cast<void *>(*it + sizeof(SizeT));
                    free_list.erase(it);
                    return ptr;
                }
            }
            return nullptr;
        }

        void *Heap::malloc(SizeT size) {
            // Try to find usable spaces in free list
            void *ptr = find_in_free_list(size);
            // If found,return.
            if (ptr != nullptr)
                return ptr;
            // Checkout remain spaces,if enough,return.
            if (hl - hp >= size + sizeof(SizeT)) {
                get_size(hp) = size;
                ptr = reinterpret_cast<void *>(hp + sizeof(SizeT));
                hp += size + sizeof(SizeT);
                return ptr;
            }
            // Compress the memory spaces
            compress();
            // Try to find usable spaces in free list again.
            ptr = find_in_free_list(size);
            // If found,return.
            if (ptr != nullptr)
                return ptr;
            else // There have no usable spaces,throw bad alloc Exception.
                throw dvm::core::Exception(DVM_BAD_ALLOC);
        }

        void Heap::free(void *ptr) {
            free_list.push_back(reinterpret_cast<Byte *>(ptr) - sizeof(SizeT));
        }
    }
}