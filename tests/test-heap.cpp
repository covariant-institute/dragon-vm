//
// Created by Mike Lee on 2017/8/29.
//

#include <core/heap.hpp>
#include <core/utils.hpp>
#include <core/config.hpp>
#include <iostream>
template<typename T>
constexpr dvm::core::mem_block_info get_mbi()
{
	return {dvm::core::type_id_converter<T>::get_type_id(), sizeof(T)};
}

int main()
{
	using dvm::core::config::HEAP_DEFAULT_SIZE;
	dvm::core::heap h(HEAP_DEFAULT_SIZE);
	auto* pa=static_cast<dvm::core::Int*>(h.malloc(get_mbi<dvm::core::Int>()));
	*pa=10;
	std::cout<<*pa<<std::endl;
	h.free(pa);
	auto* pb=static_cast<dvm::core::Char*>(h.malloc(get_mbi<dvm::core::Char>()));
	*pb='A';
	std::cout<<*pb<<std::endl;
	h.free(pb);
	return 0;
}