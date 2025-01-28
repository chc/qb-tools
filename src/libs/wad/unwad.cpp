#include "wad.h"
#include <cassert>

void wad_iterate_files(const char *hed_path, const char *wad_path, WADFileInfoCallback callback){
    FileStream hed_fd(hed_path);
    if (!hed_fd.IsFileOpened()) {
        fprintf(stderr, "Failed to open PAK: %s\n", hed_path);
        return;
    }
    hed_fd.SetReadEndian(ISTREAM_LITTLE_ENDIAN);

    WADItem item;
    item.wad_fd = fopen(wad_path, "rb");
    if(item.wad_fd == NULL) {
        return;
    }

    while(true) {
        size_t entry_offset = hed_fd.GetOffset();
        item.offset = hed_fd.ReadUInt32();
        if(item.offset == 0xffffffff) {
            break;
        }

        item.size = hed_fd.ReadUInt32();

        char ch = 0;
        int idx = 0;
        do {
            ch = hed_fd.ReadByte();
            item.filename[idx] = ch;

            idx++;
        } while(ch != 0);

        hed_fd.Align();

        callback(&item);
    }
}
void unwad_read_file(WADItem *item, uint8_t *output_buffer) {

    size_t offset = item->offset;
#ifdef WAD_SECTOR_SIZE
    offset *= WAD_SECTOR_SIZE;
#endif
    offset = (offset + 0x7FF) & ~0x7FF;

    int res = fseek((FILE *)item->wad_fd, offset, SEEK_SET);
    assert(res == 0);

    int len = fread(output_buffer, item->size, 1, (FILE *)item->wad_fd);
    assert(len == 1);
}