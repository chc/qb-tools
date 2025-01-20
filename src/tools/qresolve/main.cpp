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
        fprintf(stderr, "usage: %s [checksum] (dbgpath)\n", argv[0]);
        return -1;
    }

    const char* dbgpath = NULL;
    if (argc == 3) {
        dbgpath = argv[2];
    }

    uint32_t checksum = (uint32_t)strtoul(argv[1], NULL, 0);

    if (dbgpath != NULL) {
        printf("** loading dbg .dbg file: %s\n", dbgpath);
        dbginfo_load_dbg(dbgpath);
    }

    const char* dbg_pak_path = getenv("QBTOOLS_DBG_PAK");
    const char* dbg_pab_path = getenv("QBTOOLS_DBG_PAB");
    if (dbg_pak_path != NULL) {
        printf("** loading dbg pak\n", dbg_pak_path);
        dbginfo_load_pak(dbg_pak_path, dbg_pab_path);
    }
    else {
        printf("** no dbg data specified\n");
    }

    printf("Checksum: 0x%08x / %d\n", checksum, checksum);
    printf("Name: %s\n", dbginfo_resolve(checksum));
    return 0;
}