//
// Created by kiva on 2017/8/28.
//

#include <core/stack.hpp>
#include <core/config.hpp>
#include <cassert>
#include <core/runtime/vm_context.hpp>

int main() {
    using namespace dvm::core;
    using namespace dvm::core::object;
    using namespace dvm::core::runtime;
    VMContext context{};

    Stack s(dvm::core::config::STACK_DEFAULT_SIZE);

    auto *prototype_int32 = Class::find_class(context, "Int32");
    auto *i32 = s.new_instance(prototype_int32);
    assert(i32->prototype == prototype_int32);
    assert(i32->slots[0].object->prototype == Class::find_class(context, "Object"));

    i32->slots[1].set_i32(52019);

    auto *another_i32 = s.new_instance(prototype_int32);
    another_i32->slots[1].set_i32(1234);
    s.pop();

    i32 = s.peek();
    assert(i32->slots[1].i32 == 52019);

    return 0;
}
