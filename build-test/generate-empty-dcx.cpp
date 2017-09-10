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

UInt32 write_string_constant(FILE *fp, const char *str) {
    static UInt32 id = 0;

    UInt32 current_id = id++;
    DcxFileConstantEntry constantEntry = { };
    constantEntry.header.constant_data_size = static_cast<UInt32>(strlen(str));
    constantEntry.header.constant_id = current_id;
    constantEntry.constant_data = (Byte *) str;

    fwrite(reinterpret_cast<void *>(&constantEntry.header), sizeof(constantEntry.header), 1, fp);
    fwrite(reinterpret_cast<void *>(constantEntry.constant_data), sizeof(Byte),
           constantEntry.header.constant_data_size, fp);

    return current_id;
}

void write_class_entry(FILE *fp, UInt32 class_name_id, UInt32 class_slot_count) {
    DcxFileClassEntry classEntry = { };
    classEntry.header.class_name_id = class_name_id;
    classEntry.header.class_slot_count = class_slot_count;

    fwrite(reinterpret_cast<void *>(&classEntry.header), sizeof(classEntry.header), 1, fp);
}

void write_method_entry(FILE *fp, UInt32 method_name_id, Byte *body, SizeT length) {
    DcxFileMethodEntry methodEntry = { };
    methodEntry.header.method_length = static_cast<UInt32>(length);
    methodEntry.header.method_name_id = method_name_id;
    methodEntry.method_body = body;

    fwrite(reinterpret_cast<void *>(&methodEntry.header), sizeof(methodEntry.header), 1, fp);
    fwrite(reinterpret_cast<void *>(methodEntry.method_body), sizeof(Byte),
           methodEntry.header.method_length, fp);
}

int main(int argc, const char **argv) {
    FILE *fp = fopen(argv[1] ? argv[1] : "empty-dcx.dcx", "wb");
    if (!fp) {
        fprintf(stderr, "Failed to open dcx file: %s\n", strerror(errno));
        return 1;
    }

    DcxFileConstantPoolHeader constantPoolHeader = { };
    constantPoolHeader.constant_entries = 3;

    DcxFileClassPoolHeader classPoolHeader = { };
    classPoolHeader.class_entries = 1;

    DcxFileMethodPoolHeader methodPoolHeader = { };
    methodPoolHeader.method_entries = 1;

    DcxFileHeader header = { };
    header.version_id = make_version_id();

    fwrite(reinterpret_cast<const void *>(&header), sizeof(header), 1, fp);

    fwrite(reinterpret_cast<const void *>(&constantPoolHeader), sizeof(constantPoolHeader), 1, fp);

    fwrite(reinterpret_cast<const void *>(&classPoolHeader), sizeof(classPoolHeader), 1, fp);

    fwrite(reinterpret_cast<const void *>(&methodPoolHeader), sizeof(methodPoolHeader), 1, fp);

    // Write constant "hello_world"
    write_string_constant(fp, "hello_world");
    UInt32 class_name_id = write_string_constant(fp, "Main");
    UInt32 method_name_id = write_string_constant(fp, "dvm_main");

    // Write class "Main"
    write_class_entry(fp, class_name_id, 10086);

    // Write method "do_sth"
    SizeT length = 128;
    auto *body = (Byte *) malloc(sizeof(Byte) * length);
    bzero(reinterpret_cast<void *>(body), length);
    write_method_entry(fp, method_name_id, body, length);

    fclose(fp);
    printf("OK\n");
    return 0;
}
