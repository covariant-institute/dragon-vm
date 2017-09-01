//
// Created by kiva on 2017/9/1.
//

#include <core/object/basic_classes.hpp>

namespace dvm {
    namespace core {
        namespace object {
            void create_basic_classes() {
                Class::define_bootstrap_class("Object", 1);
            }
        }
    }
}
