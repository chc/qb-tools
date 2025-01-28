#ifndef _LIBPRE_H
#define _LIBPRE_H
#include <stdint.h>
#include <stdio.h>

class FileStream;

#define PRE_MAX_FILENAME 0xA0

#if PRE_VERSION == 2
#define PRE_VERSION 0xABCD0002
#elif PRE_VERSION == 3
#define PRE_VERSION 0xABCD0003 //version 3 is used in THUG/THUG2
#endif

typedef struct _PreItem {
    uint32_t original_size;
    uint32_t compressed_size;
    uint32_t filename_checksum;
    char filename[PRE_MAX_FILENAME];

    uint32_t data_offset;

    //internal
    FILE *file_fd;
    const char *file_path;
    _PreItem *next;
} PreItem;

typedef bool (*FileInfoCallback)(PreItem *item);

void unpre_iterate_files(const char *path, FileInfoCallback callback);
void unpre_read_file(PreItem *item, uint8_t *output_buffer);

typedef struct {
    FileStream *pre_fd;
    
    PreItem *first_pre_item;
    PreItem *last_pre_item;
    uint32_t total_files;
} PreContext;

PreContext *pre_create(const char *pre_path);
PreContext *pre_open(const char *pre_path);
void pre_append_file(PreContext *ctx, const char *path);
void pre_close(PreContext *ctx);
#endif //_LIBPRE_H