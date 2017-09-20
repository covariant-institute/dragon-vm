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
    VMContext context{ };

    Stack s(dvm::core::config::STACK_DEFAULT_SIZE);

    auto *prototype_int32 = Class::find_class(context, "Int32");

    auto *i32 = s.new_object(prototype_int32);
    assert(i32->prototype == prototype_int32);
    assert(i32->slots[0].get<Object *>()->prototype == Class::find_class(context, "Object"));
    i32->slots[1].set<Int32>(52019);

    i32 = s.peek_object();
    assert(i32->slots[1].data.i32 == 52019);
    s.pop(); // i32

    auto *obj = prototype_int32->new_instance();
    s.push_object(obj);
    s.pop(); // obj

    auto *another_i32 = s.new_object(prototype_int32);
    another_i32->slots[1].set<Int32>(1234);
    s.pop(); // another_i32

    s.push<Int32>(10);
    assert(s.peek<Int32>() == 10);
    s.pop(); // int32

    return 0;
}
