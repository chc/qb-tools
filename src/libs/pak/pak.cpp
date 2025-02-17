#include "pak.h"
#include <FileStream.h>
#include <string>
#include <cassert>
#include <crc32.h>

PakContext *pak_create(const char *pak_path, const char *pab_path) {
    if(!pak_path) {
        return nullptr;
    }

    PakContext *ctx = new PakContext;
    ctx->first_pak_item = nullptr;
    ctx->last_pak_item = nullptr;
    ctx->pak_fd = nullptr;
    ctx->pab_fd = nullptr;

    ctx->pak_fd = new FileStream(pak_path, true);
    if(!ctx->pak_fd->IsFileOpened()) {
        fprintf(stderr, "Failed to open PAK %s for writing\n", pak_path);
        return nullptr;
    }
    ctx->pak_fd->SetWriteEndian(ISTREAM_PAK_ENDIAN);

    if(pab_path == nullptr) {
        ctx->pab_fd = ctx->pak_fd;
    } else {
        ctx->pab_fd = new FileStream(pab_path, true);
        if(!ctx->pab_fd->IsFileOpened()) {
            fprintf(stderr, "Failed to open PAK %s for writing\n", pab_path);
            return nullptr;
        }        
        ctx->pab_fd->SetWriteEndian(ISTREAM_PAK_ENDIAN);
    }



    return ctx;
}

void calculate_pak_name_checksums(PakItem *item, const char *name) {
    char *tmpstr = strdup(name);
    int len = strlen(tmpstr);
    for(int i=0;i<len;i++) {
        if(tmpstr[i] == '/') {
            tmpstr[i] = '\\';
        }
    }

    char *ext = strrchr(tmpstr, '.');
    if(ext) {
        item->type = crc32(-1, ext, strlen(ext));
    }
    
    item->fullname = crc32(-1, tmpstr, strlen(tmpstr));
    
    const char *end = strrchr(tmpstr, '\\');
    if(end) {
        end = end+1;
    } else {
        end = tmpstr;
    }
    if(ext) {
       *ext = 0; 
    }
    
    item->short_name = crc32(-1, end, strlen(end));

    free(tmpstr);
    
}
void pak_append_end_file(PakContext *ctx) {
    PakItem *item = new PakItem;
    memset(item, 0, sizeof(PakItem));

    item->type = crc32(-1, PAK_LAST_ITEM, strlen(PAK_LAST_ITEM));
    item->size = 4;

    if(ctx->first_pak_item == nullptr) {
        ctx->first_pak_item = item;
        ctx->last_pak_item = item;
    } else {
        ctx->last_pak_item->next = item;
    }
    
    ctx->last_pak_item = item;
}
void pak_append_file(PakContext *ctx, const char *path) {
     FILE *fd = thps_fopen(path, "rb");
     if(!fd) {
        assert(false);
        return;
     }
    
    PakItem *item = new PakItem;
    memset(item, 0, sizeof(PakItem));
    item->file_path = strdup(path);

    fseek((FILE *)fd, 0, SEEK_END);

    size_t total_size = ftell(fd);
    item->size = total_size;
    
    fclose(fd);

   
    calculate_pak_name_checksums(item, path);

    if(ctx->first_pak_item == nullptr) {
        ctx->first_pak_item = item;
        ctx->last_pak_item = item;
    } else {
        ctx->last_pak_item->next = item;
    }
    
    ctx->last_pak_item = item;
}

void pak_close(PakContext *ctx) {
    pak_append_end_file(ctx);

    PakItem *current_item = ctx->first_pak_item;
    while(current_item != nullptr) {
        current_item->file_offset = ctx->pak_fd->GetOffset();
        ctx->pak_fd->WriteUInt32(current_item->type);
        ctx->pak_fd->WriteUInt32(current_item->offset);
        ctx->pak_fd->WriteUInt32(current_item->size);
#ifdef PAK_SWAP_FULLNAME
        ctx->pak_fd->WriteUInt32(current_item->fullname);
        ctx->pak_fd->WriteUInt32(current_item->pakname);
#else
        ctx->pak_fd->WriteUInt32(current_item->pakname);
        ctx->pak_fd->WriteUInt32(current_item->fullname);
#endif

        ctx->pak_fd->WriteUInt32(current_item->short_name);
        ctx->pak_fd->WriteUInt32(current_item->fileNameKey);
        #ifdef PAK_INCLUDE_FILENAME
        if (current_item->file_path != nullptr) {
            current_item->flags |= PAK_FLAGS_HAS_FILENAME;
        }
        #endif
        ctx->pak_fd->WriteUInt32(current_item->flags);
           

        if (current_item->flags & PAK_FLAGS_HAS_FILENAME) {
            strcpy(current_item->filename, current_item->file_path);
            ctx->pak_fd->WriteBuffer((uint8_t*) & current_item->filename, sizeof(current_item->filename));
        }

        current_item = current_item->next;
    }

    size_t new_diff = (ctx->pak_fd->GetOffset() + 0x7FF) & ~0x7FF;
    new_diff -= ctx->pak_fd->GetOffset();
    while (new_diff--) {
		ctx->pak_fd->WriteByte(0xCD);
    }
    
    //write pak data & store offset
    current_item = ctx->first_pak_item;
    while(current_item != nullptr) {
        FILE *fd = NULL;
        if(current_item->file_path != NULL) { //if null, its a padding item (like .last)
            fd = (FILE *)fopen(current_item->file_path, "rb");
        }
        
        current_item->offset = ctx->pab_fd->GetOffset();
        if(fd) {
           uint8_t *tmp = new uint8_t[current_item->size];
           int len = fread(tmp, current_item->size, 1, fd);
           assert(len == 1);
           ctx->pab_fd->WriteBuffer(tmp, current_item->size);
           delete[] tmp; 
           fclose(fd);
        } else {
            while(current_item->size--) {
                ctx->pab_fd->WriteByte(0xAB);
            }
        }

        new_diff = (ctx->pab_fd->GetOffset() + 31) & ~31;
        new_diff -= ctx->pab_fd->GetOffset();
        while (new_diff--) {
            ctx->pab_fd->WriteByte(0xCD);
        }
        
        current_item = current_item->next;
    }

    size_t pak_cursor = ctx->pak_fd->GetOffset();
    bool uses_pab = ctx->pab_fd != ctx->pak_fd;

    //update offsets
    current_item = ctx->first_pak_item;
    while(current_item != nullptr) {
        ctx->pak_fd->SetCursor(current_item->file_offset + sizeof(uint32_t));
        #ifdef PAB_ABSOLUTE_OFFSET
        //XXX: test this with single pab-less paks too
        ctx->pak_fd->WriteUInt32(
            current_item->offset
        );
        #else
            ctx->pak_fd->WriteUInt32(
                current_item->offset - current_item->file_offset + (uses_pab ? pak_cursor : 0)
            );
        #endif

        current_item = current_item->next;
    }

    new_diff = (ctx->pab_fd->GetOffset() + 0xFFF) & ~0xFFF;
    new_diff -= ctx->pab_fd->GetOffset();
    while (new_diff--) {
        ctx->pab_fd->WriteByte(0xAB);
    }

}