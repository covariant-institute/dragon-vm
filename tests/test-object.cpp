//
// Created by kiva on 2017/9/1.
//

#include <core/object/object.hpp>
#include <cassert>
#include <core/runtime/vm_context.hpp>

int main() {
    using namespace dvm::core;
    using namespace dvm::core::object;
    using namespace dvm::core::runtime;

    DragonVM vm;
    VMContext *context = vm.current_thread()->get_context();

    Object *object = context->find_class("Object")->new_instance();
    assert(object->slots[0].get<Object *>() == object);

    Object *i32 = context->find_class("Int32")->new_instance();
    assert(i32->prototype->parent == context->find_class("Object"));
    assert(i32->slots[0].get<Object *>()->prototype == context->find_class("Object"));
    assert(i32->slots[0].get<Object *>()->slots[0].get<Object *>() == i32->slots[0].get<Object *>());

    i32->slots[1].set(100);
    assert(i32->slots[1].data.i32 == 100);
    assert(i32->slots[1].get<Int32>() == 100);

    Object *u64 = context->new_UInt64(0);
    u64->slots[1].set(UINT64_MAX);
    assert(u64->slots[1].get_type() == TypeIdentifier::TYPE_ID_UINT64);
    assert(u64->slots[1].data.i64 == UINT64_MAX);
    assert(u64->slots[1].get<Int64>() == -1);

    Object *boolean = context->new_Int32(False);
    assert(boolean->slots[1].data.i32 == False);
    assert(boolean->slots[1].get<Bool>() == False);
}
