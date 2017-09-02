//
// Created by kiva on 2017/9/1.
//

#include <core/object/object.hpp>
#include <core/object/basic_classes.hpp>
#include <cassert>

int main() {
    using namespace dvm::core;
    using namespace dvm::core::object;
    create_basic_classes();

    Object *object = Class::find_class("Object")->create_object();
    assert(object->slots[0].object == object);

    Object *i32 = Class::find_class("Int32")->create_object();
    assert(i32->prototype->parent == Class::find_class("Object"));
    assert(i32->slots[0].slot_type == type_identifier::TYPE_ID_OBJECT);
    assert(i32->slots[0].object->prototype == Class::find_class("Object"));
    assert(i32->slots[0].object->slots[0].object == i32->slots[0].object);
}
