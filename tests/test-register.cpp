//
// Created by kiva on 2017/8/30.
//

#include <core/runtime/vm_register.hpp>
#include <core/runtime/vm_context.hpp>
#include <cassert>
#include <core/stack.hpp>
#include <core/config.hpp>

int main() {
    using namespace dvm::core;
    using namespace dvm::core::runtime;
    using namespace dvm::core::object;

    DragonVM vm;
    VMContext *context = vm.attachCurrentThread();

    VMRegisterHolder regs;

    auto i32 = context->find_class("Int32")->new_instance();
    i32->slots[1].set(10086);

    auto visitor = regs[0];
    visitor->set(10086);

    assert(visitor->data.i32 == 10086);

    visitor->set_unchecked(static_cast<dvm::core::UInt32>('Z'));
    assert(visitor->data.i32 == 'Z');

    Stack s(config::STACK_DEFAULT_SIZE);
    s.new_frame(256);
    s.push_object_ref(context->new_Double(3.14));
    visitor->set_unchecked(s.peek_pop<object::Object *>());
    auto obj = visitor->get<object::Object *>();
    assert(obj->slots[1].get<Double>() == 3.14);
    return 0;
}
