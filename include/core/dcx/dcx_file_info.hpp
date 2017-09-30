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
            enum DcxFileConstantType {
                CONSTANT_CLASS,
                CONSTANT_STRING,
            };

            struct DcxFileConstantPoolHeader {
                UInt16 constant_entries;
                UInt8 unused[2];
            };

            struct DcxFileConstantEntryHeader {
                UInt16 constant_id;
                UInt16 constant_type;
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
                UInt16 class_entries;
                UInt8 unused[2];
            };

            struct DcxFileClassEntryHeader {
                UInt16 class_name_id;
                UInt16 parent_class_name_id;
                UInt16 class_slot_count;
                UInt16 member_slot_count;
            };

            struct DcxFileClassEntry {
                DcxFileClassEntryHeader header;
            };

            /**
             * 方法定义
             */
            struct DcxFileMethodPoolHeader {
                UInt16 method_entries;
                UInt8 unused[2];
            };

            struct DcxFileMethodEntryHandler {
                UInt16 exception_class_name_id;
                UInt16 handler_offset;
            };

            struct DcxFileMethodEntryHeader {
                UInt16 method_name_id;
                UInt16 method_signature_id;
                UInt16 method_return_type_name_id;
                UInt16 method_handlers_count;
                UInt16 method_locals_size;
                UInt16 method_args_size;
                UInt32 method_body_size;
                Bool method_is_native;
                Bool method_is_static;
            };

            struct DcxFileMethodEntry {
                DcxFileMethodEntryHeader header;
                DcxFileMethodEntryHandler *handlers;
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
