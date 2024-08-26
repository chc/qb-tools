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

    uint32_t checksum = (uint32_t)strtol(argv[1], NULL, 0);

    const char* checksum_path = getenv("QTOOLS_DBGPAK_PATH");
    if (checksum_path != NULL) {
        printf("** loading checksum path: %s\n", checksum_path);
        
    }
    else {
        printf("** env var QTOOLS_DBGPAK_PATH is not defined.\n");
        return -1;
    }

    dbginfo_load(checksum_path, getenv("QTOOLS_CHECKSUM_FASTDUMP") != nullptr);

    printf("Checksum: 0x%08x / %d\n", checksum, checksum);
    printf("Name: %s\n", dbginfo_resolve(checksum));
    return 0;
}