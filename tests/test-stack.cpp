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
    DragonVM vm;
    VMContext *context = vm.current_thread()->get_context();

    Stack s(dvm::core::config::STACK_DEFAULT_SIZE);
    s.new_frame(256);

    auto *prototype_int32 = context->find_class("Int32");

    auto *i32 = prototype_int32->new_instance();
    assert(i32->prototype == prototype_int32);
    assert(i32->slots[0].get<Object *>()->prototype == context->find_class("Object"));
    i32->slots[1].set<Int32>(52019);

    s.push_object_ref(i32);
    i32 = s.peek_object();
    assert(i32->slots[1].data.i32 == 52019);
    s.pop<Int32>(); // i32

    // push object reference
    auto *obj = prototype_int32->new_instance();
    obj->slots[1].set<Int32>(1008611);
    s.push_object_ref(obj);
    auto *ref = s.peek_object();
    assert(ref->prototype == obj->prototype);
    assert(ref->slots[1].get<Int32>() == 1008611);
    s.pop_object(); // obj

    auto *another_i32 = prototype_int32->new_instance();
    another_i32->slots[1].set<Int32>(1234);
    s.push_object_ref(another_i32);
    s.pop_object(); // another_i32

    s.push<Int32>(10);
    assert(s.peek<Int32>() == 10);
    s.pop<Int32>(); // int32

    s.remove_top_frame();

    // test shared frame
    s.new_frame(sizeof(Int32));
    s.push<Int32>(52019);
    auto i = s.peek<Int32>();
    assert(i == 52019);

    // create a shared frame, with sizeof(Int32) bytes shared with the current one.
    s.new_frame(0, sizeof(Int32));
    i = s.peek_pop<Int32>();
    assert(i == 52019);
    s.remove_top_frame();

    s.remove_top_frame();
    return 0;
}
