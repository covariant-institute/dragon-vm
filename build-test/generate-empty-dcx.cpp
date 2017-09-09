//
// Created by kiva on 2017/9/8.
//

#include <core/dcx/dcx_file.hpp>
#include <cerrno>
#include <cstdio>
#include <cstring>

using namespace dvm::core::dcx;
using namespace dvm::core::config;

int main(int argc, const char **argv) {
    FILE *fp = fopen(argv[1] ? argv[1] : "empty-dcx.dcx", "wb");
    if (!fp) {
        fprintf(stderr, "Failed to open dcx file: %s\n", strerror(errno));
        return 1;
    }

    DcxFileConstantPoolHeader constantPoolHeader = {
            .constant_entries = 123
    };

    DcxFileClassPoolHeader classPoolHeader = {
            .class_entries = 456
    };

    DcxFileMethodPoolHeader methodPoolHeader = {
            .method_entries = 789
    };

    DcxFileHeader header = {
            .version_id = make_version_id(),
    };

    fwrite(reinterpret_cast<const void *>(&header), sizeof(header), 1, fp);

    fwrite(reinterpret_cast<const void *>(&constantPoolHeader), sizeof(constantPoolHeader), 1, fp);

    fwrite(reinterpret_cast<const void *>(&classPoolHeader), sizeof(classPoolHeader), 1, fp);

    fwrite(reinterpret_cast<const void *>(&methodPoolHeader), sizeof(methodPoolHeader), 1, fp);

    fclose(fp);

    printf("OK\n");
    return 0;
}
