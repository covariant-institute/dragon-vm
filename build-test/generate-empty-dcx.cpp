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

    dcx_file_header header = {
            .version_id = make_version_id(),
            .jump_table = {
                    .class_defs_offset = 0,
                    .constant_pool_offset = 0,
                    .method_defs_offset = 0
            }
    };

    fwrite(reinterpret_cast<const void *>(&header), sizeof(header), 1, fp);
    fclose(fp);
    return 0;
}
