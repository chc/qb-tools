#include <stdio.h>
#include <string.h>
#include <crc32.h>
#include <ctype.h>
#include <stdlib.h>
#include <dbginfo.h>
void append_checksum(const char *name, uint32_t checksum, const char *dbginfo_path) {
    DbgChecksumInfo info;
    info.checksum = checksum;
    info.name = name;

    dbginfo_append_cache(dbginfo_path, &info);
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: (options) %s [name]\n", argv[0]);
        return -1;
    }
    bool dbginfo_append = true;

    char *name = strdup(argv[1]);
    int len = strlen(name);

    uint32_t checksum = crc32(0, name, len);
    printf("Checksum (case sensitive): 0x%08x - %d\n", checksum, checksum);

    const char* dbginfo_path = getenv("QBTOOLS_DBGINFO_PATH");
    if (dbginfo_path != NULL) {
        printf("** loadin dbginfo: %s\n", dbginfo_path);
        dbginfo_load_cache(dbginfo_path);
    }
    else {
        printf("** no dbg data specified\n");
    }

    if(dbginfo_append && dbginfo_path) {
        append_checksum(name, checksum, dbginfo_path);
    }
    
    for(int i=0;i<len;i++) {
        name[i] = tolower(name[i]);
    }

    checksum = crc32(0, name, len);
    printf("Checksum (Lower): 0x%08x - %d\n", checksum, checksum);

    if(dbginfo_append && dbginfo_path) {
        append_checksum(name, checksum, dbginfo_path);
    }

    free((void *)name);

    return checksum;
}