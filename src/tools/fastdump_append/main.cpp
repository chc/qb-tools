#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <FileStream.h>
#include <map>
#include <utility>

#include <stdint.h>
#include <string.h>
#include <dbginfo.h>

extern "C" {
    extern std::map<uint32_t, DbgChecksumInfo> loaded_checksums; //dbg_pak checksums
}

#define CHUNK_SIZE (0x80000)
int main(int argc, const char* argv[]) {
    //z_verbose = 1;
    if (argc < 2) {
        fprintf(stderr, "usage: %s [dbg_pak]\n", argv[0]);
        return -1;
    }

    dbginfo_load(argv[1], 0);

    if(getenv("QTOOLS_CHECKSUM_FASTDUMP") == nullptr) {
        printf("** QTOOLS_CHECKSUM_PATH must point to fastdump file. If already pointing to this file, set QTOOLS_CHECKSUM_FASTDUMP to 1\n");
        return -1;
    }

    const char* checksum_path = getenv("QTOOLS_CHECKSUM_PATH");
    if (checksum_path != NULL) {
        printf("** appending to checksum path: %s\n", checksum_path);
        
    }
    else {
        printf("** env var QTOOLS_CHECKSUM_PATH is not defined.\n");
        return -1;
    }
    dbginfo_load(checksum_path, true);

    printf("Database contains %d checksums\n", loaded_checksums.size());

    FastDumpChecksumDumpData rec;
    std::map<uint32_t, DbgChecksumInfo>::iterator it;
    FILE *fd = fopen(checksum_path, "ab+");
    while(!feof(fd)) {
        int len = fread(&rec, sizeof(FastDumpChecksumDumpData), 1, fd);
        if(len != 1) {
            break;
        }
        it = loaded_checksums.find(rec.checksum);
        if(it != loaded_checksums.end()) {
            loaded_checksums.erase(it);
        }
        
    }

    printf("Appending %d checksums to database\n", loaded_checksums.size());
    it = loaded_checksums.begin();
    while(it != loaded_checksums.end()) {
        assert(strlen(it->second.name) < sizeof(rec.name));
        strcpy(rec.name, it->second.name);
        rec.checksum = it->first;
        printf("append: %08x - %s\n", rec.checksum, rec.name);
        fwrite(&rec, sizeof(rec), 1, fd);
        it++;
    }
    fclose(fd);
    return 0;
}