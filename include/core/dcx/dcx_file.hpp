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

            struct DcxFileMethodEntryHeader {
                UInt32 method_name_id;
                UInt32 method_length;
            };

            struct DcxFileMethodEntry {
                DcxFileMethodEntryHeader header;
                Byte *method_body;
            };

            /**
             * DCX 文件头信息
             */
            struct DcxFileHeader {
                config::VersionID version_id;
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
