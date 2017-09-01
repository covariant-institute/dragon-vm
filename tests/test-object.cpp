//
// Created by kiva on 2017/9/1.
//

#include <core/object/object.hpp>
#include <core/object/basic_classes.hpp>
#include <cassert>

int main() {
    using namespace dvm::core::object;
    create_basic_classes();
    Object *object = Class::find_class("Object")->create_object();
    assert(object->slots[0].object == object);
}
