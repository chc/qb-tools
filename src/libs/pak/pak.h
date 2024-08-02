#ifndef _LIBPAK_H
#define _LIBPAK_H
#include <stdint.h>
typedef struct _PakItem {
    uint32_t type;
    uint32_t offset;
    uint32_t size;
    uint32_t fullname;
    uint32_t pakname;
    uint32_t short_name;
    uint32_t fileNameKey;
    uint32_t flags;    

    //internal info
    uint32_t file_offset;
    uint32_t pak_file_len;
    void *pab_fd;
} PakItem;


typedef bool (*FileInfoCallback)(PakItem item);

/*
    This function will iterate all items in a pak, calling the callback. Pass null pab_path if there is no pab.
*/
void unpak_iterate_files(const char *pak_path, const char *pab_path, FileInfoCallback callback);

void unpak_read_file(PakItem item, uint8_t *output_buffer);
#endif //_LIBPAK_H