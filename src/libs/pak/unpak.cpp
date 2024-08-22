#include "pak.h"
#include <FileStream.h>
#include <string>
#include <cassert>
void unpak_iterate_files(const char *pak_path, const char *pab_path, FileInfoCallback callback) {
    FileStream pak_fd(pak_path);
    if (!pak_fd.IsFileOpened()) {
        fprintf(stderr, "Failed to open PAK: %s\n", pak_path);
        return;
    }

    pak_fd.SetReadEndian(ISTREAM_BIG_ENDIAN);

    FILE* pab_fd = NULL;
    bool created = false;
    if (pab_path == NULL || strcmp(pak_path, pab_path) == 0) {
        pab_fd = pak_fd.GetHandle();
    }
    else {
        pab_fd = fopen(pab_path, "rb");
        created = true;
        if(!pab_fd) {
            fprintf(stderr, "Failed to open PAB: %s\n", pak_path);
            return;
        }
    }

    PakItem item;
    item.pab_fd = pab_fd;
    item.pak_file_len = 0;
    item.using_pab_file = pab_path != nullptr;

    int idx = 0;
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

        size_t last_offset = pak_fd.GetOffset();

        if(idx++ == 0 && pab_path != nullptr) {
            item.pak_file_len = item.offset;
        }

        

        if (item.type == 749989691 || item.type == 0) {
            break;
        }
        if(!callback(item)) {
            break;
        }
        pak_fd.SetCursor(last_offset);
    }

    if(created) {
        fclose(pab_fd);
    }


}
void unpak_read_file(PakItem item, uint8_t *output_buffer) {
    size_t offset = item.offset + item.file_offset - item.pak_file_len;
    #ifdef PAB_ABSOLUTE_OFFSET
    if(item.using_pab_file) {
        offset = item.offset;
    }
    #endif
    
    int res = fseek((FILE *)item.pab_fd, offset, SEEK_SET);
    assert(res == 0);

    int len = fread(output_buffer, item.size, 1, (FILE *)item.pab_fd);
    assert(len == 1);
}