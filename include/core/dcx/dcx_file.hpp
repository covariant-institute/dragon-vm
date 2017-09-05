//
// Created by kiva on 2017/9/5.
//

#pragma once

#include <core/type.hpp>
#include <core/config.hpp>

namespace dvm {
    namespace core {
        namespace dcx {
            /**
             * 每个成员都存储着对应区开始位置在文件中的偏移
             */
            struct dcx_file_jump_table {
                /**
                 * 常量池
                 */
                UInt32 constant_pool_offset;

                /**
                 * 类的定义和实现
                 */
                UInt32 class_defs_offset;

                /**
                 * 程序开始执行的地方
                 */
                UInt32 entry_point_offset;
            };

            /**
             * 常量池
             */
            struct dcx_file_constant_pool_header {
                UInt32 constant_id;
                UInt32 constant_data_size;
            };

            struct dcx_file_constant_pool_entry {
                dcx_file_constant_pool_header header;
                Byte *constant_data;
            };

            /**
             * 类定义
             */
            struct dcx_file_class_header {
                UInt32 class_name_size;
                UInt32 class_data_size;
                UInt32 class_slot_count;
                UInt32 member_class_count;
            };

            struct dcx_file_class_entry {
                dcx_file_class_header header;
                Byte *class_name;
                Byte *class_data;
            };

            /**
             * DCX 文件头信息
             */
            struct dcx_file_header {
                config::VersionID version_id;
                dcx_file_jump_table jump_table;
            };
        }
    }
}
