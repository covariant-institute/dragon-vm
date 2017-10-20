//
// Created by kiva on 2017/10/10.
//

#pragma once

#include <core/type.hpp>
#include <core/object/reference.hpp>

namespace dvm {
    namespace core {
        namespace object {
            struct Class;
            struct Object;

            struct Array {
            private:
                const Class *element_type;
                SizeT length;
                Reference elements[0];

                explicit Array(const Class *element_type, SizeT length);

            public:
                Array() = delete;

                Array(const Array &array) = delete;

                ~Array() = default;

                inline SizeT get_length() const {
                    return length;
                }

                inline const Class *get_array_type() const {
                    return element_type;
                }

                inline Reference get(SizeT index) {
                    return this->elements[index];
                }

                inline bool is_null() const {
                    return element_type == nullptr;
                }

                void set(SizeT index, object::Reference ref);

                static Reference new_array(Class *element_type, SizeT length);
            };

            inline void ensure_array_valid(Array *array) {
                if (array == nullptr) {
                    throw dvm::core::Exception(DVM_INVALID_OBJECT_MEMORY);
                }
            }
        }
    }
}
