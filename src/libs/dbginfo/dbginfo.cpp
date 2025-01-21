#include "dbginfo.h"
#include <stdint.h>
#include <pak.h>
#include <stdio.h>
#include <cassert>
#include <string.h>
#include <map>



extern "C" {
    std::map<uint32_t, const char *> loaded_checksums;
}

bool dbginfo_load_cache(const char* path) {
    FILE* fd = fopen(path, "rb");
    if(!fd) {
        return false;
    }

    char input[1024];
    while(!feof(fd)) {
        uint32_t checksum;
        int len = fread(&checksum, sizeof(uint32_t), 1, fd);
        if(len != 1) break;

        uint32_t string_len;
        len = fread(&string_len, sizeof(uint32_t), 1, fd);
        if(len != 1) break;

        if(string_len > sizeof(input)) break;

        len = fread(&input, string_len, 1, fd);
        input[string_len] = 0;

        loaded_checksums[checksum] = strdup(input);
    }
    fclose(fd);
    return true;
}

bool dbginfo_append_cache(const char* path, DbgChecksumInfo *info) {
    if(loaded_checksums.find(info->checksum) != loaded_checksums.end()) {
        return false;
    }
    loaded_checksums[info->checksum] = strdup(info->name);

    FILE *fd = fopen(path, "ab");
    if(!fd) return false;

    uint32_t len = strlen(info->name);

    fseek(fd, 0, SEEK_END);

    fwrite(&info->checksum, sizeof(uint32_t), 1, fd);
    fwrite(&len, sizeof(uint32_t), 1, fd);
    fwrite(info->name, len, 1, fd);

    fclose(fd);
    return true;
}

const char *dbginfo_resolve(uint32_t checksum) {
    if(loaded_checksums.find(checksum) != loaded_checksums.end()) {
        return loaded_checksums[checksum];
    }
    return nullptr;
}
