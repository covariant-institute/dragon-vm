//
// Created by kiva on 2017/10/16.
//

#include <core/object/reference.hpp>
#include <core/object/object.hpp>
#include <core/object/array.hpp>

namespace dvm {
    namespace core {
        namespace object {

            Object *Reference::as_object() {
                if (!is_object()) {
                    throw dvm::core::Exception(DVM_OBJECT_UNSATISFIED_REF);
                }
                return refs.ref_o;
            }

            Array *Reference::as_array() {
                if (!is_array()) {
                    throw dvm::core::Exception(DVM_OBJECT_UNSATISFIED_REF);
                }
                return refs.ref_a;
            }

            const Class *Reference::get_reference_prototype() const {
                if (is_array()) {
                    return refs.ref_a->get_array_type();
                }
                if (is_object()) {
                    return refs.ref_o->get_prototype();
                }

                throw dvm::core::Exception(DVM_OBJECT_UNSATISFIED_REF);
            }

            Reference Reference::make_object(Object *object) {
                Reference ref{ };
                ref.ref_type = ReferenceType::OBJECT;
                ref.refs.ref_o = object;
                return ref;
            }

            Reference Reference::make_array(Array *array) {
                Reference ref{ };
                ref.ref_type = ReferenceType::ARRAY;
                ref.refs.ref_a = array;
                return ref;
            }

            bool Reference::operator==(const Reference &other) const {
                return other.get_type() == this->get_type()
                        && other.refs.ref_o == this->refs.ref_o
                        && other.refs.ref_a == this->refs.ref_a;
            }

            bool Reference::is_reference_null() {
                if (is_object()) {
                    auto *obj = as_object();
                    object::ensure_object_valid(obj);
                    return obj->is_null();
                }

                if (is_array()) {
                    auto *array = as_array();
                    object::ensure_array_valid(array);
                    return array->is_null();
                }

                throw dvm::core::Exception(DVM_OBJECT_UNSATISFIED_REF);
            }
        }
    }
}
