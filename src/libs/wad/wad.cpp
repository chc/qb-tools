#include "wad.h"
#include <string.h>
#include <cassert>

WADContext *wad_create(const char *hed_path, const char *wad_path) {
    if(!hed_path || !wad_path) {
        return nullptr;
    }
    WADContext *ctx = new WADContext;

    //create HED
    ctx->hed_fd = new FileStream(hed_path, true);
    if(!ctx->hed_fd->IsFileOpened()) {
        fprintf(stderr, "Failed to open HED %s for writing\n", hed_path);
        return nullptr;
    }
    ctx->hed_fd->SetWriteEndian(ISTREAM_LITTLE_ENDIAN);


    //create WAD
    ctx->wad_fd = new FileStream(wad_path, true);
    if(!ctx->wad_fd->IsFileOpened()) {
        fprintf(stderr, "Failed to open WAD %s for writing\n", wad_path);
        return nullptr;
    }
    ctx->wad_fd->SetWriteEndian(ISTREAM_LITTLE_ENDIAN);

    return ctx;
}

void wad_append_file(WADContext *ctx, const char *path) {
     FILE *fd = thps_fopen(path, "rb");
     if(!fd) {
        assert(false);
        return;
     }
    
    WADItem *item = new WADItem;
    memset(item, 0, sizeof(WADItem));
    item->file_path = strdup(path);

    fseek((FILE *)fd, 0, SEEK_END);

    size_t total_size = ftell(fd);
    item->size = total_size;
    
    fclose(fd);


    if(ctx->first_item == nullptr) {
        ctx->first_item = item;
        ctx->last_item = item;
    } else {
        ctx->last_item->next = item;
    }
    ctx->last_item = item;
}

static size_t write_to_wad(WADContext *ctx, WADItem *item) {
    size_t new_diff = (ctx->wad_fd->GetOffset() + 0x7FF) & ~0x7FF;
    new_diff -= ctx->wad_fd->GetOffset();
    while (new_diff--) {
		ctx->wad_fd->WriteByte(0);
    }

    size_t offset = ctx->wad_fd->GetOffset();

    FILE *fd = thps_fopen(item->file_path, "rb");
    
    uint8_t *tmp = new uint8_t[item->size];
    int len = fread(tmp, item->size, 1, fd);
    assert(len == 1);

    ctx->wad_fd->WriteBuffer(tmp, item->size);
    
    delete[] tmp;
    fclose(fd);
    
    return offset;
    
}
void wad_close(WADContext *ctx) {
    WADItem *current_item = ctx->first_item;
    while(current_item != nullptr) {
        FILE *fd = thps_fopen(current_item->file_path, "rb");
        if(!fd) {
            current_item = current_item->next;
            continue;
        }

        size_t offset = write_to_wad(ctx, current_item);
        #ifdef WAD_SECTOR_SIZE
            offset /= WAD_SECTOR_SIZE;
        #endif

        size_t size = current_item->size;

        ctx->hed_fd->WriteUInt32(offset);
        ctx->hed_fd->WriteUInt32(size);
        ctx->hed_fd->WriteNTS(current_item->file_path);
        ctx->hed_fd->WriteAlign();

        fclose(fd);
        current_item = current_item->next;
    }

    //write terminator
    ctx->hed_fd->WriteUInt32(0xffffffff);
    ctx->hed_fd->WriteAlign();
}