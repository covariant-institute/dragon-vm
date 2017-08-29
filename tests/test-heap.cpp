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
template<typename T> T* gc_malloc()
{
	T* ptr= static_cast<T*>(default_heap.malloc(get_mbi<T>()));
	default_stack.push_memory(get_mbi<dvm::core::Link >(),&ptr);
}
void gc_start()
{
	default_heap.gc(default_stack);
}

int main()
{
	using dvm::core::config::HEAP_DEFAULT_SIZE;
	dvm::core::heap h(HEAP_DEFAULT_SIZE);
	auto* pa=gc_malloc<dvm::core::Int >();
//	*pa=10;
//	std::cout<<*pa<<std::endl;
	auto* pb=gc_malloc<dvm::core::Char>();
//	*pb= 'A';
//	std::cout<<*pb<<std::endl;
	gc_start();
	return 0;
}