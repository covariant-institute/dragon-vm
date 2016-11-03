#include <iostream>
#include <string>
#include "memory.hpp"
int main()
{
	constexpr cov::heap::size_t heap_size=32;
	cov::heap mem(heap_size,cov::heap::allocate_policy::first_fit);
	int* ptr0=static_cast<int*>(mem.malloc(sizeof(int)));
	cov::construct(ptr0,10);
	std::cout<<*ptr0<<std::endl;
	cov::destroy(ptr0);
	mem.free(ptr0);
	double* ptr1=static_cast<double*>(mem.malloc(sizeof(double)));
	cov::construct(ptr1,3.141);
	std::cout<<*ptr1<<std::endl;
	cov::destroy(ptr1);
	mem.free(ptr1);
	void* ptr3=mem.malloc(heap_size-sizeof(cov::heap::size_t));
	mem.free(ptr3);
	return 0;
}
