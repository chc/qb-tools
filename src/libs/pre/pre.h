#ifndef _LIBPRE_H
#define _LIBPRE_H
#include <stdint.h>
#include <stdio.h>

#define PRE_VERSION 0xABCD0003
typedef struct _PreItem {
    uint32_t original_size;
    uint32_t compressed_size;
    uint32_t data_checksum;
    char filename[0xA0];

    uint32_t data_offset;

    //internal
    FILE *file_fd;
} PreItem;

typedef bool (*FileInfoCallback)(PreItem item);


//void unpak_iterate_files(const char *pak_path, const char *pab_path, FileInfoCallback callback);

void unpre_iterate_files(const char *path, FileInfoCallback callback, bool alignment_hack = false);
void unpre_read_file(PreItem *item, uint8_t *output_buffer);
#endif //_LIBPRE_H