
#include <iostream>
#include "stack.hpp"

int main() {
    dvm::stack s(32);
    auto a = static_cast<dvm::Int>('A');

    dvm::Float b[] = {3.14, 6.28};
    s.push({0, sizeof(dvm::Int)}, &a);
    s.push({0, sizeof(dvm::Float[2])}, b);

    auto *pb = static_cast<dvm::Float *>(s.top());
    std::cout << pb[0] + pb[1] << std::endl;
    s.pop();

    auto *pa = static_cast<dvm::Char *>(s.top());
    std::cout << *pa << std::endl;
    return 0;
}
