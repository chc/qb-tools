#include "dbginfo.h"
#include <stdint.h>
#include <pak.h>
#include <stdio.h>
#include <cassert>
#include <string.h>
#include <vector>

typedef struct _DbgChecksumInfo {
    uint32_t checksum;
    const char *name;
} DbgChecksumInfo;

std::vector<DbgChecksumInfo> loaded_checksums;

void handle_line(const char *line) {
    DbgChecksumInfo line_info;

    line_info.checksum = strtol(line, NULL, 0);

    line += 11;

    line_info.name = strdup(line);
    loaded_checksums.push_back(line_info);
}

void handle_dbgfile(uint8_t *data, uint32_t len) {
    const char *match = "[Checksums]\r\n";
    int match_idx = 0;

    uint8_t *p = data;
    while(len--) {
        if(match[match_idx] == 0) {
            break; //found expected data
        }
        if(*p == match[match_idx]) {
            match_idx++;
        }
        p++;
    }

    assert(match_idx != 0);

    while(len) {
        char *end = strchr((char *)p, '\r');

        if(end == NULL) break;
        *end = 0;

        int line_len = (end - (char *)p) + 2;
        if(line_len <= 2) break;


        handle_line((const char *)p);
        len -= line_len;
        p = (uint8_t*)end + 2;
    }
}

bool pak_file_info_callback(PakItem item) {
    uint8_t* buf = new uint8_t[item.size];
    unpak_read_file(item, buf);

    handle_dbgfile(buf, item.size);

    delete[] buf;

    return true;
}

void dbginfo_load(const char *dbgpak) {
    unpak_iterate_files(dbgpak, nullptr, pak_file_info_callback);
}
const char *dbginfo_resolve(uint32_t checksum) {
    std::vector<DbgChecksumInfo>::iterator it = loaded_checksums.begin();
    while(it != loaded_checksums.end()) {
        if(it->checksum == checksum) {
            return it->name;
        }
        it++;
    }
    return nullptr;
}