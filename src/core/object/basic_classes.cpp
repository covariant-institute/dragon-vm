//
// Created by kiva on 2017/9/1.
//

#include <core/object/basic_classes.hpp>

namespace dvm {
    namespace core {
        namespace object {
            void create_basic_classes() {
                Class *object = Class::define_bootstrap_class("Object", 1);
                Class::define_class(object, "Int8", 2);
                Class::define_class(object, "Int16", 2);
                Class::define_class(object, "Int32", 2);
                Class::define_class(object, "Int64", 2);
                Class::define_class(object, "UInt8", 2);
                Class::define_class(object, "UInt16", 2);
                Class::define_class(object, "UInt32", 2);
                Class::define_class(object, "UInt64", 2);
                Class::define_class(object, "Bool", 2);
                Class::define_class(object, "Byte", 2);
                Class::define_class(object, "Float", 2);
                Class::define_class(object, "Double", 2);
            }
        }
    }
}
