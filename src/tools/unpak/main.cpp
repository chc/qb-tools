#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <FileStream.h>
#include <map>
#include <vector>

#include <string.h>
#include <pak.h>
#include <dbginfo.h>

std::vector<PakItem> pak_items;

const char* get_checksum(uint32_t key) {
    return dbginfo_resolve(key);
}

#define PRINT_FIELD(name, var) c = get_checksum(var); \
if(c) { \
    printf("%s: %s\n", name, c); \
} \
else {\
    printf("%s: %d\n",name,var); \
} 


void print_pak_item(PakItem* item) {


    const char* c = NULL;
    printf("**** BEGIN ITEM ****\n");
    PRINT_FIELD("Type", item->type);
    PRINT_FIELD("Fullname", item->fullname);
    PRINT_FIELD("Pak Name", item->pakname);
    PRINT_FIELD("Short Name Name", item->short_name);
    PRINT_FIELD("fileNameKey", item->fileNameKey);
    PRINT_FIELD("flags", item->flags);
    printf("Offset: %08x - %d\n", item->offset, item->offset);
    printf("Expected End Offset: %08x - %d\n", item->size + item->offset, item->size + item->offset);
    printf("Size: %08x - %d\n", item->size, item->size);

    printf("\n");

    assert(item->flags == 0);
}


bool unpak_file_info_callback(PakItem item) {
    print_pak_item(&item);

    uint8_t* buf = new uint8_t[item.size];
    unpak_read_file(item, buf);

    const char* path = get_checksum(item.pakname);

    if (path) {
        char *name = strdup(path);
        for(int i=0;i<strlen(name);i++) {
            if(name[i] == '\\') {
                name[i] = '_';
            }
        }

        FILE* out = fopen(name, "wb");
        if (out) {
            fwrite(buf, item.size, 1, out);
            fclose(out);
        }
        free(name);
    } else {
        assert(false);
    }

    delete[] buf;

    return true;
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s [pak_path] (pab_path)\n", argv[0]);
        return -1;
    }

    const char *dbginfo_path = getenv("QTOOLS_DBGPAK_PATH");
    if(dbginfo_path != NULL) {
        printf("** loading dbginfo path: %s\n", dbginfo_path);
        dbginfo_load(dbginfo_path);
    } else {
        printf("** checksum path not specified\n");
    }


    unpak_iterate_files(argv[1], argv[2], unpak_file_info_callback);
    
    
    return 0;
}