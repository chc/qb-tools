

#ifndef _WAD_H
#define _WAD_H

#include <stdint.h>
#include <FileStream.h>

#define MAX_WAD_NAME 128
typedef struct _WADItem {
    char filename[MAX_WAD_NAME];
    size_t offset;
    size_t size;

    //internal info
    _WADItem *next;
    FILE *wad_fd;
} WADItem;


typedef struct {
    FileStream *hed_fd;
    FileStream *wad_fd;
    
    WADItem *first_item;
    WADItem *last_item;
} WADContext;

typedef bool (*WADFileInfoCallback)(WADItem *item);

/*
    This function will iterate all items in a pak, calling the callback. Pass null pab_path if there is no pab.
*/
void wad_iterate_files(const char *pak_path, const char *pab_path, WADFileInfoCallback callback);
void unwad_read_file(WADItem *item, uint8_t *output_buffer);


WADContext *wad_create(const char *hed_path, const char *wad_path);
void wad_append_file(WADContext *ctx, const char *path);
void wad_close(WADContext *ctx);
#endif //_LIBPAK_H
