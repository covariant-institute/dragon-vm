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
            struct DcxFileJumpTable {
                /**
                 * 常量池
                 */
                UInt32 constant_pool_start;

                /**
                 * 类的定义和实现
                 */
                UInt32 class_pool_start;

                /**
                 * 方法定义和实现
                 */
                UInt32 method_pool_start;
            };

            /**
             * 常量池
             */
            struct DcxFileConstantPoolHeader {
                UInt32 constant_entries;
            };

            struct DcxFileConstantEntryHeader {
                UInt32 constant_id;
                UInt32 constant_data_size;
            };

            struct DcxFileConstantEntry {
                DcxFileConstantEntryHeader header;
                Byte *constant_data;
            };

            /**
             * 类定义
             */
            struct DcxFileClassPoolHeader {
                UInt32 class_entries;
            };

            struct DcxFileClassEntryHeader {
                UInt32 class_name_id;
                UInt32 class_slot_count;
                UInt32 member_class_count;
            };

            struct DcxFileClassEntry {
                DcxFileClassEntryHeader header;
            };

            /**
             * 方法定义
             */
            struct DcxFileMethodPoolHeader {
                UInt32 method_entries;
            };

            struct DcxFileMethodHeader {
                UInt32 method_name_id;
                UInt32 method_length;
            };

            struct DcxFileMethodEntry {
                DcxFileMethodHeader header;
                Byte *method_body;
            };

            /**
             * DCX 文件头信息
             */
            struct DcxFileHeader {
                config::VersionID version_id;
                DcxFileJumpTable jump_table;
            };

            struct DcxFileInfo {
                DcxFileHeader file_header;

                DcxFileConstantPoolHeader constant_pool_header;
                DcxFileClassPoolHeader class_pool_header;
                DcxFileMethodPoolHeader method_pool_header;
            };
        }
    }
}
