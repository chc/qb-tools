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

void dbginfo_load_pak(const char* pak, const char* pab);
void dbginfo_load_dbg(const char* path);
const char *dbginfo_resolve(uint32_t checksum);
#endif //_DBGINFO_H
