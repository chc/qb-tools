#include "dbginfo.h"
#include <stdint.h>
#include <pak.h>
#include <stdio.h>
#include <cassert>
#include <string.h>
#include <map>



extern "C" {
    std::map<uint32_t, DbgChecksumInfo> loaded_checksums;
}

void handle_line(const char *line) {
    DbgChecksumInfo line_info;

    line_info.checksum = strtol(line, NULL, 0);

    line += 11; //skip checksum (fixed length)

    line_info.name = strdup(line);
    loaded_checksums[line_info.checksum] = line_info;
}

void handle_dbgfile(uint8_t *data, uint32_t len) {
    const char *match = "[Checksums]\r\n";

    uint8_t* p = (uint8_t*)strstr((char *)data, match);

    assert(p != nullptr);

    len -= (p - data);

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


void dbginfo_load(const char *dbgpak, bool is_fastdump) {
    if (is_fastdump) {
        FILE* fd = fopen(dbgpak, "rb");
        if (fd == NULL) {
            return;
        }
        while (true) {
            FastDumpChecksumDumpData dump;
            int len = fread(&dump, sizeof(FastDumpChecksumDumpData), 1, fd);
            if (len != 1) {
                break;
            }
            DbgChecksumInfo line_info;
            line_info.checksum = dump.checksum;
            line_info.name = strdup(dump.name);
            loaded_checksums[line_info.checksum] = line_info;
        }
        fclose(fd);
    }
    else {
        unpak_iterate_files(dbgpak, nullptr, pak_file_info_callback);
    }
    
}
const char *dbginfo_resolve(uint32_t checksum) {
    if (loaded_checksums.find(checksum) == loaded_checksums.end()) {
        return nullptr;
    }
    return loaded_checksums[checksum].name;
}
