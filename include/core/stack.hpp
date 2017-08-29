#pragma once

#include <core/type.hpp>
#include <core/memory.hpp>
#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>
#include <cstring>
#include <cstdlib>

namespace dvm {
	namespace core {
		class stack final {
			void *allocated_start = nullptr;
			Byte *stack_pointer = nullptr;

		private:
			inline void move_forward(size_t forward_size)
			{
				stack_pointer += forward_size;
			}

			inline void move_backward(size_t backward_size)
			{
				stack_pointer -= backward_size;
			}

		public:
			stack() = delete;

			explicit stack(ULong stack_size) : allocated_start(malloc(stack_size))
			{
				if(allocated_start==nullptr)
					throw dvm::core::exception(DVM_BAD_ALLOC);

				memset(allocated_start, '\0', stack_size);
				stack_pointer = static_cast<Byte *>(allocated_start);
			}

			stack(const stack &) = delete;

			~stack()
			{
				if (allocated_start != nullptr) {
					free(allocated_start);
				}
			}

			void push_memory(const mem_block_info &mbi, const void *dat)
			{
				memcpy(stack_pointer, dat, mbi.data_size);
				move_forward(mbi.data_size);
				memcpy(stack_pointer, (void *) &mbi, sizeof(mem_block_info));
				move_forward(sizeof(mem_block_info));
			}

			void pop()
			{
				move_backward(sizeof(mem_block_info));
				mem_block_info *mbi = reinterpret_cast<mem_block_info *>(stack_pointer);
				move_backward(mbi->data_size);
			}

			void *top()
			{
				void *target_addr = stack_pointer - sizeof(mem_block_info);
				mem_block_info *mbi = reinterpret_cast<mem_block_info *>(target_addr);
				return stack_pointer - sizeof(mem_block_info) - mbi->data_size;
			}

			template <typename T>
			void push(const T &t)
			{
				type_identifier type_id = type_id_converter<T>::get_type_id();

				// TODO: Support non-primitive type
				if(type_id != type_identifier::TYPE_ID_UNSPECIFIC)
					throw dvm::core::exception(DVM_TYPE_ID_UNSPECIFIC);

					mem_block_info block_info = { type_id, sizeof(T) };
					push_memory(block_info, &t);
				}

			template <typename T>
			void pushArray(const T *array, size_t length)
			{
				type_identifier type_id = type_id_converter<T>::get_type_id();

				// TODO: Support non-primitive type
				if(type_id != type_identifier::TYPE_ID_UNSPECIFIC)
					throw dvm::core::exception(DVM_TYPE_ID_UNSPECIFIC);

					mem_block_info block_info = { type_id, static_cast<Addr>(sizeof(T) * length) };
					push_memory(block_info, array);
				}
		};
	}
}
