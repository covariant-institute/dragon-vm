//
// Created by kiva on 2017/8/28.
//

#include <core/stack.hpp>
#include <core/config.hpp>
#include <core/object/basic_classes.hpp>
#include <cassert>

int main() {
    using namespace dvm::core;
    using namespace dvm::core::object;

    init_base_classes();
    Stack s(dvm::core::config::STACK_DEFAULT_SIZE);

    auto *prototype_int32 = Class::find_class("Int32");
    auto *i32 = s.new_instance(prototype_int32);
    assert(i32->prototype == prototype_int32);
    assert(i32->slots[0].slot_type == type_identifier::TYPE_ID_OBJECT);
    assert(i32->slots[0].object->prototype == Class::find_class("Object"));
    assert(i32->slots[1].slot_type == type_identifier::TYPE_ID_UNSPECIFIC);

    i32->slots[1].set_i32(52019);
    assert(i32->slots[1].slot_type == type_identifier::TYPE_ID_INT32);

    auto *another_i32 = s.new_instance(prototype_int32);
    another_i32->slots[1].set_i32(1234);
    s.pop();

    i32 = s.peek();
    assert(i32->slots[1].i32 == 52019);

    return 0;
}
