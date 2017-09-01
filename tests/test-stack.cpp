//
// Created by kiva on 2017/8/28.
//

#include <core/stack.hpp>
#include <core/utils.hpp>
#include <core/config.hpp>
#include <iostream>

int main()
{
	using dvm::core::config::STACK_DEFAULT_SIZE;

	dvm::core::stack s(STACK_DEFAULT_SIZE);

	s.push(dvm::core::Int32('A'));

	dvm::core::Float b[] = {3.14, 6.28};
	s.pushArray(b, array_length(b));

	auto *pb = static_cast<dvm::core::Float *>(s.top());
	std::cout << pb[0] + pb[1] << std::endl;
	s.pop();

	auto *pa = static_cast<dvm::core::Int8 *>(s.top());
	std::cout << *pa << std::endl;
	s.pop();
	return 0;
}
