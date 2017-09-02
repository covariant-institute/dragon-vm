//
// Created by kiva on 2017/9/1.
//

#include <core/object/basic_classes.hpp>

namespace dvm {
    namespace core {
        namespace object {
            void create_basic_classes() {
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
        }
    }
}
