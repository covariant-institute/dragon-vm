//
// Created by Mike Lee on 2017/8/29.
//

#include <core/heap.hpp>
#include <core/stack.hpp>
#include <core/utils.hpp>
#include <core/config.hpp>
#include <iostream>

template<typename T>
constexpr dvm::core::mem_block_info get_mbi()
{
	return {dvm::core::type_id_converter<T>::get_type_id(), sizeof(T), dvm::core::False};
}

dvm::core::stack default_stack(dvm::core::config::STACK_DEFAULT_SIZE);
dvm::core::heap default_heap(dvm::core::config::HEAP_DEFAULT_SIZE);

template<typename T>
T *gc_malloc()
{
	using namespace dvm::core;
	Byte *ptr = reinterpret_cast<Byte *>(default_heap.malloc(get_mbi<T>())) - sizeof(mem_block_info);
	default_stack.push_memory(get_mbi<Link>(), &ptr);
	return reinterpret_cast<T *>(ptr + sizeof(mem_block_info));
}

void free_top()
{
	default_heap.free(default_stack.top());
	default_stack.pop();
}

void gc_start()
{
	default_heap.gc(default_stack);
}

int main()
{
	auto *pa = gc_malloc<dvm::core::Int>();
	*pa = 10;
	std::cout << *pa << std::endl;
	auto *pb = gc_malloc<dvm::core::Char>();
	*pb = 'A';
	std::cout << *pb << std::endl;
	free_top();
	std::cout << "GC Start" << std::endl;
	gc_start();
	return 0;
}