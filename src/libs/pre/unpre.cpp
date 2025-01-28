#include "pre.h"
#include <stdlib.h>
#include <cassert>
#include <FileStream.h>
#include <stdio.h>
#include <lzss.h>

void unpre_iterate_files(const char *path, FileInfoCallback callback) {
    FileStream pre_fd(path);
    if (!pre_fd.IsFileOpened()) {
        fprintf(stderr, "Failed to open PRE: %s\n", path);
        return;
    }

    pre_fd.SetReadEndian(ISTREAM_PRE_ENDIAN);

    uint32_t total_size = pre_fd.ReadUInt32();
    uint32_t version = pre_fd.ReadUInt32();
    assert(version == PRE_VERSION);
    uint32_t num_files = pre_fd.ReadUInt32();

    PreItem item;
    item.file_fd = pre_fd.GetHandle();
    for(int i=0;i<num_files;i++) {

        item.original_size = pre_fd.ReadUInt32();
        item.compressed_size = pre_fd.ReadUInt32();
        uint32_t filename_len = pre_fd.ReadUInt32();
        #if PRE_VERSION == 3
            item.filename_checksum = pre_fd.ReadUInt32();
        #endif
        
        //read file name
        assert(filename_len < sizeof(item.filename));
        for(int i=0;i<filename_len;i++) {
            item.filename[i] = pre_fd.ReadByte();
        }

        item.data_offset = pre_fd.GetOffset();

        callback(item);

        int skip_size;

        if(item.compressed_size == 0) {
            skip_size = item.original_size;
        } else {
            skip_size = item.compressed_size;
        }

        skip_size = (skip_size + 3) & (~3);
        
        pre_fd.SetCursor(item.data_offset + skip_size);
    }
}
void unpre_read_file(PreItem *item, uint8_t *output_buffer) {
    if(item->compressed_size == 0) {
        //uncompressed, just read it
        int len = fread(output_buffer, sizeof(uint8_t), item->original_size, item->file_fd);
        assert(len == item->original_size);
    } else {
        uint8_t *comp_buff = (uint8_t*)malloc(item->compressed_size);
        int len = fread(comp_buff, sizeof(uint8_t), item->compressed_size, item->file_fd);
        assert(len == item->compressed_size);
        decompress_lzss(comp_buff, item->compressed_size, output_buffer);
        free(comp_buff);
    }
}