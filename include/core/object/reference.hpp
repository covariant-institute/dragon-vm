//
// Created by kiva on 2017/10/19.
//

#pragma once

#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>

namespace dvm {
    namespace core {
        namespace object {
            struct Object;
            struct Array;
            struct Class;

            enum class ReferenceType {
                OBJECT = 1, ARRAY,
            };

            struct Reference {
            private:
                ReferenceType ref_type;
                union {
                    Object *ref_o;
                    Array *ref_a;
                } refs;

                inline ReferenceType get_type() const {
                    return ref_type;
                }

            public:
                inline bool is_array() const {
                    return get_type() == ReferenceType::ARRAY;
                }

                inline bool is_object() const {
                    return get_type() == ReferenceType::OBJECT;
                }

                bool is_reference_null();

                bool operator==(const Reference &other) const;

                Object *as_object();

                Array *as_array();

                const Class *get_reference_prototype() const;

                static Reference make_object(Object *object);

                static Reference make_array(Array *array);
            };
        }
    }
}

