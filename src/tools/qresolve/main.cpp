#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <FileStream.h>
#include <map>
#include <vector>

#include <string.h>
#include <dbginfo.h>

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s [checksum]\n", argv[0]);
        return -1;
    }

    uint32_t checksum = (uint32_t)strtoul(argv[1], NULL, 0);

    const char* dbginfo_path = getenv("QBTOOLS_DBGINFO_PATH");
    if (dbginfo_path != NULL) {
        fprintf(stderr, "** loadin dbginfo: %s\n", dbginfo_path);
        dbginfo_load_cache(dbginfo_path);
    }
    else {
        fprintf(stderr, "** no dbg data specified\n");
    }

    fprintf(stderr, "Checksum: 0x%08x / %d\n", checksum, checksum);
    fprintf(stderr, "Name: %s\n", dbginfo_resolve(checksum));
    return 0;
}