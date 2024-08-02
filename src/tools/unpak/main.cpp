#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <FileStream.h>
#include <map>
#include <vector>

#include <string.h>
std::map<uint32_t, const char*> m_checksum_names;

typedef struct {
    uint32_t checksum;
    char checksum_name[64];
} ChecksumTableEntry;
void LoadChecksums(const char* path) {
    ChecksumTableEntry entry;
    FILE* fd = fopen(path, "rb");
    if (!fd) {
        return;
    }
    while (true) {
        if (feof(fd)) {
            break;
        }
        int len = fread(&entry, sizeof(ChecksumTableEntry), 1, fd);
        if (len != 1) {
            break;
        }

        if (m_checksum_names.find(entry.checksum) == m_checksum_names.end()) {
            if (m_checksum_names[entry.checksum] == NULL) {
                m_checksum_names[entry.checksum] = strdup(entry.checksum_name);
            }
        }
    }
    fclose(fd);
}


typedef struct _PakItem {
    uint32_t type;
    uint32_t offset;
    uint32_t size;
    uint32_t fullname;
    uint32_t pakname;
    uint32_t short_name;
    uint32_t fileNameKey;
    uint32_t flags;    
    uint32_t file_offset;
} PakItem;
std::vector<PakItem> pak_items;


const char* get_checksum(uint32_t key) {
    if (m_checksum_names.find(key) != m_checksum_names.end()) {
        return m_checksum_names[key];
    }
    return NULL;
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


void dump_pak_item(PakItem item, int pak_size, FILE* fd) {
    int offset = item.offset + item.file_offset - pak_size;

    fseek(fd, offset, SEEK_SET);

    uint8_t* d = new uint8_t[item.size];
    fread(d, item.size, 1, fd);

    const char* path = get_checksum(item.short_name);

    if (path) {
        FILE* out = fopen(path, "wb");
        if (out) {
            fwrite(d, item.size, 1, out);
            fclose(out);
        }
    }



    delete[] d;
}

int main(int argc, const char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s [pak_path] [pab_path]\n", argv[0]);
        return -1;
    }


    FileStream pak_fd(argv[1]);

    if (!pak_fd.IsFileOpened()) {
        fprintf(stderr, "Failed to open PAK: %s\n", argv[1]);
        return -1;
    }

    pak_fd.SetReadEndian(ISTREAM_BIG_ENDIAN);

    const char* checksum_path = getenv("QTOOLS_CHECKSUM_PATH");
    if (checksum_path != NULL) {
        printf("** loading checksum path: %s\n", checksum_path);
        LoadChecksums(checksum_path);
    }
    else {
        printf("** checksum path not specified\n");
    }


    PakItem item;
    while (true) {
        item.file_offset = pak_fd.GetOffset();
        item.type = pak_fd.ReadUInt32();
        item.offset = pak_fd.ReadUInt32();
        item.size = pak_fd.ReadUInt32();
        item.fullname = pak_fd.ReadUInt32();
        item.pakname = pak_fd.ReadUInt32();
        item.short_name = pak_fd.ReadUInt32();
        item.fileNameKey = pak_fd.ReadUInt32();
        item.flags = pak_fd.ReadUInt32();

        pak_items.push_back(item);

        if (item.type == 749989691) {
            break;
        }
    }

    FILE* fd = pak_fd.GetHandle();
    while (!feof(fd)) {
        char c;
        fread(&c, 1, 1, fd);
    }

    int offset = ftell(fd);

    FILE* pab_fd = NULL;
    bool created = false;
    if (strcmp(argv[1], argv[2]) == 0) {
        offset = 0;
        pab_fd = pak_fd.GetHandle();
    }
    else {
        pab_fd = fopen(argv[2], "rb");
    }
   


    std::vector<PakItem>::iterator it = pak_items.begin();
    while (it != pak_items.end()) {
        PakItem item = *it;
        print_pak_item(&item);
        dump_pak_item(item, offset, pab_fd);
        it++;
    }

    

    if (pab_fd != pak_fd.GetHandle()) {
        fclose(pab_fd);
    }
    
    
    return 0;
}