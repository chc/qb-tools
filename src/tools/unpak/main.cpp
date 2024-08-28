#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <FileStream.h>
#include <map>
#include <vector>

#include <string.h>
#include <pak.h>
#include <dbginfo.h>

#include <sys/stat.h>
#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

std::vector<PakItem> pak_items;

const char* get_checksum(uint32_t key) {
    return dbginfo_resolve(key);
}

#define PRINT_FIELD(name, var) c = get_checksum(var); \
if(c) { \
    printf("%s: %s / %d\n", name, c, var); \
} \
else {\
    printf("%s: %d\n",name,var); \
} 


void print_pak_item(PakItem* item) {


    const char* c = NULL;
    printf("**** BEGIN ITEM ****\n");
    printf("File offset: %08x\n", item->file_offset);
    PRINT_FIELD("Type", item->type);
    PRINT_FIELD("Fullname", item->fullname);
    PRINT_FIELD("Pak Name", item->pakname);
    PRINT_FIELD("Short Name Name", item->short_name);
    PRINT_FIELD("fileNameKey", item->fileNameKey);
    PRINT_FIELD("flags", item->flags);
    if (item->flags & PAK_FLAGS_HAS_FILENAME) {
        printf("filename: %s\n", item->filename);
    }
    printf("Offset: %08x - %d\n", item->offset, item->offset);
    printf("Expected End Offset: %08x - %d\n", item->size + item->offset, item->size + item->offset);
    printf("Size: %08x - %d\n", item->size, item->size);
    

    printf("\n");
}

void create_dir(char *path) {
    char *p = (char *)path;
    char* x = p;
    while (*x) {
        if (*x == '\\') {
            *x = '/';
        }
        x++;
    }

    while(true) {
        char *x = strchr(p, '/');
        if(x == NULL) {
            break;
        }
        *x = 0;
#ifdef _WINDOWS

        CreateDirectoryA(path, NULL);
#else
        mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
        p = x+1;
        *x = '/';

        
    }
}


bool unpak_file_info_callback(PakItem item) {
    print_pak_item(&item);

    uint8_t* buf = new uint8_t[item.size];
    unpak_read_file(item, buf);

    const char* path = get_checksum(item.pakname);

    const char* open_path = path;

    if (open_path) {
        if (strstr(open_path, "/data/")) {
            open_path = strstr(open_path, "/data/") + strlen("/data/");
        }
    }



    char *name;
    if (item.flags & PAK_FLAGS_HAS_FILENAME) {
        open_path = item.filename;
        name = strdup(open_path);
        create_dir(name);
    }
    else {
        if (open_path) {
            name = strdup(open_path);
            create_dir(name);

        }
        else {
            char tmp[32];
            const char* ext = get_checksum(item.type);
            if (ext == NULL) {
                snprintf(tmp, sizeof(tmp), "%08x.bin", item.pakname);
            }
            else {
                snprintf(tmp, sizeof(tmp), "%08x%s",item.pakname, ext);
            }
            
            name = strdup(tmp);
        }
    }





    FILE* out = fopen(name, "wb");
    if (out) {
        fwrite(buf, item.size, 1, out);
        fclose(out);
    } else {
        fprintf(stderr, "Failed to open for writing: %s\n", name);
    }

    free(name);

    delete[] buf;

    return true;
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s [pak_path] (pab_path)\n", argv[0]);
        return -1;
    }

    const char *dbginfo_path = getenv("QBTOOLS_CHECKSUM_PATH");
    if(dbginfo_path != NULL) {
        printf("** loading dbginfo path: %s\n", dbginfo_path);
        dbginfo_load(dbginfo_path, getenv("QBTOOLS_CHECKSUM_FASTDUMP") != nullptr);
    } else {
        printf("** checksum path not specified\n");
    }


    unpak_iterate_files(argv[1], argv[2], unpak_file_info_callback);
    
    
    return 0;
}