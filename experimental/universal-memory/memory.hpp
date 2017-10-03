#pragma once

#include <stdexcept>
#include <cstdint>
#include <cstdlib>
#include <utility>
#include <list>

namespace cov {
    class stack final {
    public:
        using byte=uint8_t;
        using size_t=uint64_t;
    private:
        // stack Start
        void *ss = nullptr;
        // stack Pointer
        byte *sp = nullptr;
        // stack Limit
        byte *sl = nullptr;
    public:
        stack() = delete;

        stack(const stack &) = delete;

        stack(size_t size) : ss(::malloc(size)) {
            sp = reinterpret_cast<byte *>(ss) + size;
            sl = sp;
        }

        ~stack() {
            ::free(ss);
        }

        inline bool empty() const {
            return sp == sl;
        }

        void *top() {
            if (sp == sl)
                throw std::runtime_error("stack is empty.");
            return reinterpret_cast<void *>(sp + sizeof(size_t));
        }

        void *push(size_t size) {
            if (sp - reinterpret_cast<byte *>(ss) < size + sizeof(size_t))
                throw std::runtime_error("stack overflow.");
            sp -= size + sizeof(size_t);
            *reinterpret_cast<size_t *>(sp) = size;
            return reinterpret_cast<void *>(sp + sizeof(size_t));
        }

        void pop() {
            if (sp == sl)
                throw std::runtime_error("stack is empty.");
            sp += *reinterpret_cast<size_t *>(sp) + sizeof(size_t);
        }

        size_t size_of(void *ptr) {
            return *reinterpret_cast<size_t *>(reinterpret_cast<byte *>(ptr) - sizeof(size_t));
        }
    };

    class heap final {
    public:
        using byte=uint8_t;
        using size_t=uint64_t;
        enum class allocate_policy {
            first_fit, best_fit, worst_fit
        };
    private:
        // heap Start
        void *hs = nullptr;
        // heap Pointer
        byte *hp = nullptr;
        // heap Limit
        byte *hl = nullptr;
        allocate_policy policy = allocate_policy::best_fit;
        std::list<byte *> free_list;

        inline size_t &get_size(byte *ptr) {
            return *reinterpret_cast<size_t *>(ptr);
        }

        inline void sort_by_size() {
            free_list.sort([this](byte *lhs, byte *rhs) {
                return get_size(lhs) < get_size(rhs);
            });
        }

        inline void sort_by_size_reverse() {
            free_list.sort([this](byte *lhs, byte *rhs) {
                return get_size(lhs) > get_size(rhs);
            });
        }

        inline void sort_by_addr() {
            free_list.sort([this](byte *lhs, byte *rhs) {
                return lhs < rhs;
            });
        }

        void compress() {
            // Sort the spaces by address.
            sort_by_addr();
            std::list<byte *> new_list;
            byte *ptr = nullptr;
            // Compress the free list.
            for (auto p:free_list) {
                if (ptr != nullptr) {
                    size_t &size = get_size(ptr);
                    if (ptr + size + sizeof(size_t) == p) {
                        size += get_size(p) + sizeof(size_t);
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

        void *find_in_free_list(size_t size) {
            if (!free_list.empty()) {
                switch (policy) {
                    case allocate_policy::first_fit:
                        break;
                    case allocate_policy::best_fit:
                        sort_by_size();
                        break;
                    case allocate_policy::worst_fit:
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
                    void *ptr = reinterpret_cast<void *>(*it + sizeof(size_t));
                    free_list.erase(it);
                    return ptr;
                }
            }
            return nullptr;
        }

    public:
        heap() = delete;

        heap(const heap &) = delete;

        explicit heap(size_t size, allocate_policy p = allocate_policy::best_fit) : hs(::malloc(size)), policy(p) {
            hp = reinterpret_cast<byte *>(hs);
            hl = hp + size;
        }

        ~heap() {
            ::free(hs);
        }

        void *malloc(size_t size) {
            // Try to find usable spaces in free list
            void *ptr = find_in_free_list(size);
            // If found,return.
            if (ptr != nullptr)
                return ptr;
            // Checkout remain spaces,if enough,return.
            if (hl - hp >= size + sizeof(size_t)) {
                get_size(hp) = size;
                ptr = reinterpret_cast<void *>(hp + sizeof(size_t));
                hp += size + sizeof(size_t);
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
                throw std::runtime_error("Bad alloc.");
        }

        void free(void *ptr) {
            free_list.push_back(reinterpret_cast<byte *>(ptr) - sizeof(size_t));
        }

        size_t size_of(void *ptr) {
            return get_size(reinterpret_cast<byte *>(ptr));
        }
    };

    template <typename T, typename...ArgsT>
    void construct(T *ptr, ArgsT &&...args) {
        if (ptr != nullptr)
            ::new(ptr) T(std::forward<ArgsT>(args)...);
    }

    template <typename T>
    void destroy(T *ptr) {
        if (ptr != nullptr)
            ptr->~T();
    }
}
