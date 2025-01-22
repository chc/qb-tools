#include "pre.h"
#include <stdlib.h>
#include <cassert>
#include <FileStream.h>
#include <stdio.h>
#include <lzss.h>
#include <string.h>
#include <crc32.h>
#include <ctype.h>

PreContext *pre_create(const char *pre_path) {
    if(!pre_path) {
        return nullptr;
    }

    PreContext *ctx = new PreContext;
    ctx->first_pre_item = nullptr;
    ctx->last_pre_item = nullptr;
    ctx->pre_fd = nullptr;

    ctx->pre_fd = new FileStream(pre_path, true);
    if(!ctx->pre_fd->IsFileOpened()) {
        fprintf(stderr, "Failed to open PAK %s for writing\n", pre_path);
        return nullptr;
    }
    ctx->pre_fd->SetWriteEndian(ISTREAM_PRE_ENDIAN);

    ctx->pre_fd->WriteUInt32(sizeof(uint32_t) * 3); //total size
    ctx->pre_fd->WriteUInt32(PRE_VERSION); //version PRE_VERSION
    ctx->pre_fd->WriteUInt32(0); //total files

    ctx->total_files = 0;

    return ctx;
}
PreContext *pre_open(const char *pre_path) {
    if(!pre_path) {
        return nullptr;
    }

    PreContext *ctx = new PreContext;
    ctx->first_pre_item = nullptr;
    ctx->last_pre_item = nullptr;
    ctx->pre_fd = nullptr;

    ctx->pre_fd = new FileStream(pre_path, false);
    if(!ctx->pre_fd->IsFileOpened()) {
        fprintf(stderr, "Failed to open PAK %s for writing\n", pre_path);
        return nullptr;
    }
    ctx->pre_fd->SetWriteEndian(ISTREAM_PRE_ENDIAN);

    uint32_t total_size = ctx->pre_fd->ReadUInt32();
    uint32_t version = ctx->pre_fd->ReadUInt32();
    assert(version == PRE_VERSION);
    ctx->total_files = ctx->pre_fd->ReadUInt32();

    ctx->pre_fd->SetCursor(total_size);

    return ctx; 
}
void pre_append_file(PreContext *ctx, const char *path) {
     FILE *fd = fopen(path, "rb");
     if(!fd) {
        assert(false);
        return;
     }
    
    PreItem *item = new PreItem;
    memset(item, 0, sizeof(PreItem));
    item->file_path = strdup(path);

    fseek((FILE *)fd, 0, SEEK_END);

    size_t total_size = ftell(fd);
    item->original_size = total_size;
    
    fseek(fd, 0, SEEK_SET);
    fclose(fd);


    if(ctx->first_pre_item == nullptr) {
        ctx->first_pre_item = item;
        ctx->last_pre_item = item;
    } else {
        ctx->last_pre_item->next = item;
    }
    ctx->last_pre_item = item;
}
uint32_t gen_checksum(const char* name) {
	int len = strlen(name);
    uint32_t checksum = -1;
    while (len--) {
        char c = *name;
        c = tolower(c);
        if(c == '/') { //path checksums must have backslash
            c = '\\';
        }
        checksum = crc32(checksum, &c, 1);
        name++;
    }
	return checksum;
}
void pre_close(PreContext *ctx) {
    PreItem *current_item = ctx->first_pre_item;
    while(current_item != nullptr) {
        FILE *fd = fopen(current_item->file_path, "rb");
        if(!fd) {
            current_item = current_item->next;
            continue;
        }
        ctx->pre_fd->WriteUInt32(current_item->original_size);
        

        uint8_t *input_buffer = (uint8_t *)malloc(current_item->original_size);
        uint8_t *compress_buffer = (uint8_t *)malloc(current_item->original_size * 2);

        int len = fread(input_buffer, 1, current_item->original_size, fd);
        assert(len == current_item->original_size);

        uint32_t comp_len = compress_lzss(input_buffer, current_item->original_size, compress_buffer);

        if(comp_len >= current_item->original_size) {
            comp_len = 0;
        }

        ctx->pre_fd->WriteUInt32(comp_len); //compressed size;


        uint32_t pad_len;
        uint32_t namelen = strlen(current_item->file_path);

        pad_len = (namelen + 4) & ~3;
        ctx->pre_fd->WriteUInt32(pad_len);
        pad_len -= namelen;
        

        uint32_t checksum = gen_checksum(current_item->file_path);
        ctx->pre_fd->WriteUInt32(checksum); //checksum

        ctx->pre_fd->WriteBuffer((uint8_t *)current_item->file_path, namelen);

        while (pad_len--) {
			ctx->pre_fd->WriteByte(0);
        }

        if(comp_len > 0) {
            ctx->pre_fd->WriteBuffer(compress_buffer, comp_len);
            pad_len = (comp_len + 3) & (~3);
            pad_len -= comp_len;
        } else {
            ctx->pre_fd->WriteBuffer(input_buffer, current_item->original_size);
            pad_len = (current_item->original_size + 3) & (~3);
            pad_len -= current_item->original_size;
        }

        while (pad_len--) {
            ctx->pre_fd->WriteByte(0);
        }
        

        free(input_buffer);
        free(compress_buffer);
        fclose(fd);

        current_item = current_item->next;
        ctx->total_files++;
    }

    uint32_t total_size = ctx->pre_fd->GetOffset();
    ctx->pre_fd->SetCursor(0);

    ctx->pre_fd->WriteUInt32(total_size); //total size
    ctx->pre_fd->WriteUInt32(PRE_VERSION);
    ctx->pre_fd->WriteUInt32(ctx->total_files); //total files
}