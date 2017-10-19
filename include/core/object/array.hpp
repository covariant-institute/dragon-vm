//
// Created by kiva on 2017/10/10.
//

#pragma once

#include <core/type.hpp>
#include <core/object/object.hpp>

namespace dvm {
    namespace core {
        namespace object {
            struct Array {
            private:
                SizeT length;
                const Class *element_type;
                Object *elements[0];

                explicit Array(const Class *element_type, SizeT length);

            public:
                Array() = delete;

                Array(const Array &array) = delete;

                ~Array() = default;

                inline SizeT get_length() const {
                    return length;
                }

                inline const Class *get_type() const {
                    return element_type;
                }

                inline Object* get(SizeT index) {
                    return this->elements[index];
                }

                inline void set(SizeT index, Object *object) {
                    if (object->get_prototype() != element_type) {
                        // TODO throw cast exception
                        return;
                    }
                    this->elements[index] = object;
                }

                static Array *new_array(Class *element_type, SizeT length);
            };
        }
    }
}
