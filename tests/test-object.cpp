//
// Created by kiva on 2017/9/1.
//

#include <core/object/object.hpp>
#include <core/object/basic_classes.hpp>
#include <cassert>

int main() {
    using namespace dvm::core;
    using namespace dvm::core::object;
    init_base_classes();

    Object *object = Class::find_class("Object")->new_instance();
    assert(object->slots[0].object == object);

    Object *i32 = Class::find_class("Int32")->new_instance();
    assert(i32->prototype->parent == Class::find_class("Object"));
    assert(i32->slots[0].slot_type == type_identifier::TYPE_ID_OBJECT);
    assert(i32->slots[0].object->prototype == Class::find_class("Object"));
    assert(i32->slots[0].object->slots[0].object == i32->slots[0].object);

    assert(i32->slots[1].slot_type == type_identifier::TYPE_ID_UNSPECIFIC);

    i32->slots[1].set_i32(100);
    assert(i32->slots[1].slot_type == type_identifier::TYPE_ID_INT32);
    assert(i32->slots[1].i32 == 100);

    Object *u64 = new_UInt64(UINT64_MAX);
    assert(u64->slots[1].slot_type == type_identifier::TYPE_ID_UINT64);
    assert(u64->slots[1].u64 == UINT64_MAX);

    Object *boolean = new_Bool(False);
    assert(boolean->slots[1].u8 == False);
}
