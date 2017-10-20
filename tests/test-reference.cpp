//
// Created by kiva on 2017/10/16.
//

#include <core/object/reference.hpp>
#include <core/runtime/dvm.hpp>
#include <core/runtime/context.hpp>
#include <core/object/box.hpp>
#include <cassert>
#include <core/object/array.hpp>

using namespace dvm::core;
using namespace dvm::core::object;
using namespace dvm::core::runtime;

int main() {
    DragonVM vm;
    VMContext *context = vm.current_thread()->get_context();

    auto array = Array::new_array(context->find_class("Int32"), 10);
    array.as_array()->set(0, box(context, 100));

    assert(array.as_array()->get_length() == 10);
    assert(array.get_reference_prototype() == context->find_class("Int32"));
    assert(array.is_array());

    auto ref_o = box(context, 3.14);
    assert(unbox<Double>(ref_o) == 3.14);
}
