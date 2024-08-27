#ifndef _DBGINFO_H
#define _DBGINFO_H
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define CHECKSUM_MAX 300

typedef struct _DbgChecksumInfo {
    uint32_t checksum;
    const char* name;
} DbgChecksumInfo;

typedef struct _FastDumpChecksumDumpData {
    uint32_t checksum;
    char name[CHECKSUM_MAX];
} FastDumpChecksumDumpData; //fastdump info

void dbginfo_load(const char *dbgpak, bool is_fastdump);
const char *dbginfo_resolve(uint32_t checksum);
#endif //_DBGINFO_H
