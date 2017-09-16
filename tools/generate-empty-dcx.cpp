//
// Created by kiva on 2017/9/8.
//

#include <core/dcx/dcx_file_info.hpp>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <cstdlib>

using namespace dvm::core;
using namespace dvm::core::dcx;
using namespace dvm::core::config;

UInt16 write_string_constant(FILE *fp, const char *str, bool is_class) {
    static UInt16 id = 0;

    UInt16 current_id = id++;
    DcxFileConstantEntry constantEntry = { };
    constantEntry.header.constant_data_size = static_cast<UInt32>(strlen(str));
    constantEntry.header.constant_id = current_id;
    constantEntry.header.constant_type = is_class ? CONSTANT_CLASS : CONSTANT_STRING;
    constantEntry.constant_data = (Byte *) str;

    fwrite(reinterpret_cast<void *>(&constantEntry.header), sizeof(constantEntry.header), 1, fp);
    fwrite(reinterpret_cast<void *>(constantEntry.constant_data), sizeof(Byte),
           constantEntry.header.constant_data_size, fp);

    return current_id;
}

void write_class_entry(FILE *fp, UInt16 parent_class_name_id, UInt16 class_name_id, UInt16 class_slot_count,
                       UInt16 member_slot_count) {
    DcxFileClassEntry classEntry = { };
    classEntry.header.parent_class_name_id = parent_class_name_id;
    classEntry.header.class_name_id = class_name_id;
    classEntry.header.class_slot_count = class_slot_count;
    classEntry.header.member_slot_count = member_slot_count;

    fwrite(reinterpret_cast<void *>(&classEntry.header), sizeof(classEntry.header), 1, fp);
}

void
write_method_entry(FILE *fp, UInt16 return_type_id, UInt16 method_name_id, UInt16 method_signature_id, Bool is_native,
                   Bool is_static, Byte *body, SizeT length) {
    DcxFileMethodEntry methodEntry = { };
    methodEntry.header.method_is_native = is_native;
    methodEntry.header.method_is_static = is_static;
    methodEntry.header.method_return_type_name_id = return_type_id;
    methodEntry.header.method_name_id = method_name_id;
    methodEntry.header.method_signature_id = method_signature_id;
    methodEntry.header.method_body_size = !is_native ? static_cast<UInt32>(length) : 0;

    fwrite(reinterpret_cast<void *>(&methodEntry.header), sizeof(methodEntry.header), 1, fp);

    if (!is_native) {
        methodEntry.method_body = body;
        fwrite(reinterpret_cast<void *>(methodEntry.method_body), sizeof(Byte),
               methodEntry.header.method_body_size, fp);
    }
}

int main(int argc, const char **argv) {
    FILE *fp = fopen(argv[1] ? argv[1] : "empty-dcx.dcx", "wb");
    if (!fp) {
        fprintf(stderr, "Failed to open dcx file: %s\n", strerror(errno));
        return 1;
    }

    DcxFileConstantPoolHeader constantPoolHeader = { };
    constantPoolHeader.constant_entries = 7;

    DcxFileClassPoolHeader classPoolHeader = { };
    classPoolHeader.class_entries = 1;

    DcxFileMethodPoolHeader methodPoolHeader = { };
    methodPoolHeader.method_entries = 2;

    DcxFileHeader header = { };
    header.version_id = make_version_id();

    fwrite(reinterpret_cast<const void *>(&header), sizeof(header), 1, fp);

    fwrite(reinterpret_cast<const void *>(&constantPoolHeader), sizeof(constantPoolHeader), 1, fp);

    fwrite(reinterpret_cast<const void *>(&classPoolHeader), sizeof(classPoolHeader), 1, fp);

    fwrite(reinterpret_cast<const void *>(&methodPoolHeader), sizeof(methodPoolHeader), 1, fp);

    // Write constant "hello_world"
    write_string_constant(fp, "hello_world", false);
    UInt16 class_name_id = write_string_constant(fp, "Main", true);
    UInt16 parent_class_name_id = write_string_constant(fp, "Object", true);
    UInt16 return_type_id = write_string_constant(fp, "Int32", true);
    UInt16 method_name_id = write_string_constant(fp, "dvm_main", false);
    UInt16 method_signature_id = write_string_constant(fp, "(X)", false);
    UInt16 method_signature_2_id = write_string_constant(fp, "(N)", false);

    // Write class "Main"
    write_class_entry(fp, parent_class_name_id, class_name_id, 0, 1);

    // Write method "dvm_main"
    SizeT length = 128;
    auto *body = (Byte *) malloc(sizeof(Byte) * length);
    bzero(reinterpret_cast<void *>(body), length);
    // dvm method
    write_method_entry(fp, return_type_id, method_name_id, method_signature_id, False, True, body, length);
    // native method
    write_method_entry(fp, return_type_id, method_name_id, method_signature_2_id, True, True, nullptr, 0);

    fclose(fp);
    printf("OK\n");
    return 0;
}
