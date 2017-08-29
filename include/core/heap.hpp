#pragma once

#include <core/type.hpp>
#include <core/memory.hpp>
#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <list>

namespace dvm {
	namespace core {
		class heap final {
			static inline bool compare(Byte* lhs, Byte* rhs)
			{
				return reinterpret_cast<mem_block_info*>(lhs)->data_size< reinterpret_cast<mem_block_info*>(rhs)->data_size;
			}
			void *allocated_start = nullptr;
			Byte *heap_pointer = nullptr;
			std::list<Byte*> free_list;
			inline void move_forward(size_t forward_size)
			{
				heap_pointer += forward_size;
			}
			inline void move_backward(size_t backward_size)
			{
				heap_pointer -= backward_size;
			}
		public:
			heap()=delete;
			explicit heap(ULong heap_size) : allocated_start(::malloc(heap_size))
			{
				if(allocated_start==nullptr)
					throw dvm::core::exception(DVM_BAD_ALLOC);
				memset(allocated_start, '\0', heap_size);
				heap_pointer = static_cast<Byte *>(allocated_start);
			}
			heap(const heap&)=delete;
			~heap()
			{
				if (allocated_start != nullptr) {
					::free(allocated_start);
				}
			}
			void* malloc(const mem_block_info& mbi)
			{
				if(!free_list.empty()) {
					free_list.sort(compare);
					auto it=free_list.begin();
					for(; it!=free_list.end();)
						if(reinterpret_cast<mem_block_info*>(*it)->data_size>=mbi.data_size)
							break;
					if(it!=free_list.end()) {
						Byte* cur=*it;
						reinterpret_cast<mem_block_info*>(cur)->type_id=mbi.type_id;
						void* ptr= static_cast<void*>(cur+ sizeof(mem_block_info));
						free_list.erase(it);
						return ptr;
					}
				}
				else {
					memcpy(heap_pointer, (void *) &mbi, sizeof(mem_block_info));
					move_forward(sizeof(mem_block_info));
					void* ptr=static_cast<void*>(heap_pointer);
					move_backward(mbi.data_size);
					return ptr;
				}
			}
			void free(void* ptr)
			{
				Byte * orig=static_cast<Byte *>(ptr)- sizeof(mem_block_info);
				memset(ptr,'\0',reinterpret_cast<mem_block_info*>(orig)->data_size);
				free_list.push_back(orig);
			}
		};
	}
}