//
// Created by kiva on 2017/9/1.
//

#include <core/object/basic_classes.hpp>
#include <core/object/object.hpp>

namespace dvm {
    namespace core {
        namespace object {
            void init_base_classes() {
                Class *object = Class::define_bootstrap_class("Object", 0, 1);
                Class::define_class(object, "Int8", 0, 2);
                Class::define_class(object, "Int16", 0, 2);
                Class::define_class(object, "Int32", 0, 2);
                Class::define_class(object, "Int64", 0, 2);
                Class::define_class(object, "UInt8", 0, 2);
                Class::define_class(object, "UInt16", 0, 2);
                Class::define_class(object, "UInt32", 0, 2);
                Class::define_class(object, "UInt64", 0, 2);
                Class::define_class(object, "Bool", 0, 2);
                Class::define_class(object, "Byte", 0, 2);
                Class::define_class(object, "Float", 0, 2);
                Class::define_class(object, "Double", 0, 2);
            }

#define CREATOR_SIGNATURE(TYPE) \
            Object* new_##TYPE(TYPE value)

#define CREATOR_COMMON_IMPLEMENT(TYPE, SLOT_SETTER) \
            CREATOR_SIGNATURE(TYPE) { \
                Object *object = Class::find_class(#TYPE)->new_instance(); \
                ensure_object_valid(object); \
                object->slots[1].set_##SLOT_SETTER(value); \
                return object; \
            }

            CREATOR_COMMON_IMPLEMENT(Int8, i8);

            CREATOR_COMMON_IMPLEMENT(Int16, i16);

            CREATOR_COMMON_IMPLEMENT(Int32, i32);

            CREATOR_COMMON_IMPLEMENT(Int64, i64);

            CREATOR_COMMON_IMPLEMENT(UInt8, u8);

            CREATOR_COMMON_IMPLEMENT(UInt16, u16);

            CREATOR_COMMON_IMPLEMENT(UInt32, u32);

            CREATOR_COMMON_IMPLEMENT(UInt64, u64);

            CREATOR_COMMON_IMPLEMENT(Float, f32);

            CREATOR_COMMON_IMPLEMENT(Double, f64);

            CREATOR_COMMON_IMPLEMENT(Byte, byte);

            CREATOR_COMMON_IMPLEMENT(Bool, bool);

#undef CREATOR_COMMON_IMPLEMENT

#undef CREATOR_SIGNATURE
        }
    }
}
