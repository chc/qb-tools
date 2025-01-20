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

//manage cache files
bool dbginfo_load_cache(const char* path); //load cache file into loaded checksums
bool dbginfo_append_cache(const char* path, DbgChecksumInfo *info); //append contents of loaded checksums to file

//load from old style .qb token dump file
bool dbginfo_load_qtoken(const char* path);
//load from .pre with old style qbs
bool dbginfo_cache_pre(const char* pre);

//load from thaw+ style pak
bool dbginfo_cache_pak(const char* pak, const char* pab);
bool dbginfo_load_dbg(const char* path);

const char *dbginfo_resolve(uint32_t checksum);
#endif //_DBGINFO_H
