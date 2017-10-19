//
// Created by kiva on 2017/10/19.
//

#include <core/exceptions.hpp>
#include <core/errorcodes.hpp>
#include <core/object/object.hpp>
#include <core/object/array.hpp>

namespace dvm {
    namespace core {
        namespace object {

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

            public:
                ReferenceType get_type() const {
                    return ref_type;
                }

                Object *as_object() {
                    if (ref_type != ReferenceType::OBJECT) {
                        throw dvm::core::Exception(DVM_OBJECT_UNSATISFIED_REF);
                    }
                    return refs.ref_o;
                }

                Array *as_array() {
                    if (ref_type != ReferenceType::ARRAY) {
                        throw dvm::core::Exception(DVM_OBJECT_UNSATISFIED_REF);
                    }
                    return refs.ref_a;
                }

                static Reference make(Object *object) {
                    Reference ref{ };
                    ref.ref_type = ReferenceType::OBJECT;
                    ref.refs.ref_o = object;
                    return ref;
                }

                static Reference make(Array *array) {
                    Reference ref{ };
                    ref.ref_type = ReferenceType::ARRAY;
                    ref.refs.ref_a = array;
                    return ref;
                }
            };
        }
    }
}

