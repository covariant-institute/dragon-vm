#include <iostream>
#include "memory.hpp"
void test_stack()
{
	constexpr cov::stack::size_t stack_size=32;
	cov::stack mem(stack_size);
	int* ptr0=static_cast<int*>(mem.push(sizeof(int)));
	cov::construct(ptr0,10);
	std::cout<<*ptr0<<std::endl;
	cov::destroy(ptr0);
	mem.pop();
	double* ptr1=static_cast<double*>(mem.push(sizeof(double)));
	cov::construct(ptr1,3.141);
	std::cout<<*ptr1<<std::endl;
	cov::destroy(ptr1);
	mem.pop();
	void* ptr3=mem.push(stack_size-sizeof(cov::stack::size_t));
	mem.pop();
}
void test_heap()
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
}
int main()
{
	test_stack();
	test_heap();
	return 0;
}
