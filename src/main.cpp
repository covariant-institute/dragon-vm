
#include <iostream>
#include "stack.hpp"
#include "utils.hpp"
#include "config.hpp"

int main() {
    dvm::stack s(dvm::config::STACK_DEFAULT_SIZE);
    auto a = static_cast<dvm::Int>('A');

    dvm::Float b[] = { 3.14, 6.28 };

//    s.push_memory({ dvm::type_id_converter<dvm::Int>::get_type_id(), sizeof(dvm::Int) }, &a);
    s.push(a);

//    s.push_memory({ dvm::type_id_converter<dvm::Float>::get_type_id(), sizeof(dvm::Float[2]) }, b);
    s.pushArray(b, array_length(b));

    auto *pb = static_cast<dvm::Float *>(s.top());
    std::cout << pb[0] + pb[1] << std::endl;
    s.pop();

    auto *pa = static_cast<dvm::Char *>(s.top());
    std::cout << *pa << std::endl;
    s.pop();
    return 0;
}
