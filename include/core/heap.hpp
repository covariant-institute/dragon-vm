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
		class heap final {
			static inline bool compare(Byte *lhs, Byte *rhs)
			{
				return reinterpret_cast<mem_block_info *>(lhs)->data_size < reinterpret_cast<mem_block_info *>(rhs)->data_size;
			}

			void *allocated_start = nullptr;
			Byte *heap_pointer = nullptr;
			std::list<Byte *> free_list;

			inline void move_forward(size_t forward_size)
			{
				heap_pointer += forward_size;
			}

			inline void move_backward(size_t backward_size)
			{
				heap_pointer -= backward_size;
			}

		public:
			heap() = delete;

			explicit heap(ULong heap_size) : allocated_start(::malloc(heap_size))
			{
				if (allocated_start == nullptr)
					throw dvm::core::exception(DVM_BAD_ALLOC);
				memset(allocated_start, '\0', heap_size);
				heap_pointer = static_cast<Byte *>(allocated_start);
			}

			heap(const heap &) = delete;

			~heap()
			{
				if (allocated_start != nullptr) {
					::free(allocated_start);
				}
			}

			void *malloc(const mem_block_info &);

			void free(void *);

			void gc(const stack &);
		};
	}
}