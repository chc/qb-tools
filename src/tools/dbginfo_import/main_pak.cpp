#include <stdio.h>
#include <string.h>
#include <crc32.h>
#include <ctype.h>
#include <stdlib.h>
#include <dbginfo.h>
#include <pak.h>
#include <cassert>

const char* dbginfo_path = NULL;

void handle_line(const char *line) {
    DbgChecksumInfo line_info;

    line_info.checksum = strtol(line, NULL, 0);

    line += 11; //skip checksum (fixed length)

    line_info.name = line;

    dbginfo_append_cache(dbginfo_path, &line_info);
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

bool unpak_file_info_callback(PakItem *item) {
    uint8_t* buf = new uint8_t[item->size];
    unpak_read_file(item, buf);

    handle_dbgfile(buf, item->size);

    delete[] buf;
    return true;
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: [pak_path]\n", argv[0]);
        return -1;
    }

    dbginfo_path = getenv("QBTOOLS_DBGINFO_PATH");
    if (dbginfo_path != NULL) {
        dbginfo_load_cache(dbginfo_path);
    }
    else {
        printf("** no dbg data specified\n");
        return -1;
    }

    unpak_iterate_files(argv[1], nullptr, unpak_file_info_callback);
    return 0;
}