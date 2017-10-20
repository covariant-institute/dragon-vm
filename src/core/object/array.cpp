//
// Created by kiva on 2017/10/16.
//

#include <core/object/array.hpp>
#include <core/object/object.hpp>
#include <core/memory.hpp>

namespace dvm {
    namespace core {
        namespace object {

            Array::Array(const Class *element_type, SizeT length)
                    : element_type(element_type), length(length) {
                memset(elements, '\0', sizeof(elements[0]) * length);
            }

            Reference Array::new_array(Class *element_type, SizeT length) {
                auto *uninitialized = dvm_malloc(sizeof(Array) + sizeof(Object *) * length);
                auto *array = new(uninitialized) Array(element_type, length);
                return Reference::make_array(array);
            }

            void Array::set(SizeT index, Reference ref) {
                if (ref.get_reference_prototype() != element_type) {
                    // TODO throw cast exception
                    return;
                }
                this->elements[index] = ref;
            }
        }
    }
}
