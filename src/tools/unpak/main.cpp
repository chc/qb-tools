#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <FileStream.h>


#include <string.h>



void show_dump(unsigned char *data, unsigned int len, FILE *stream) {
    static const char       hex[] = "0123456789abcdef";
    static unsigned char    buff[67];   /* HEX  CHAR\n */
    unsigned char           chr,
                            *bytes,
                            *p,
                            *limit,
                            *glimit = data + len;

    memset(buff + 2, ' ', 48);

    while(data < glimit) {
        limit = data + 16;
        if(limit > glimit) {
            limit = glimit;
            memset(buff, ' ', 48);
        }

        p     = buff;
        bytes = p + 50;
        while(data < limit) {
            chr = *data;
            *p++ = hex[chr >> 4];
            *p++ = hex[chr & 15];
            p++;
            *bytes++ = ((chr < ' ') || (chr >= 0x7f)) ? '.' : chr;
            data++;
        }
        *bytes++ = '\n';

        fwrite(buff, bytes - buff, 1, stream);
    }
}

typedef struct _PakItem {
    uint32_t type;
    uint32_t offset;
    uint32_t size;
    uint32_t unk2;
    uint32_t name;
    uint32_t short_name;
    uint32_t unk[2];
} PakItem;

int decompress_lzss(unsigned char *g_infile, int g_infilesz, unsigned char *g_outfile);


void dump_buffer(const char *path, uint8_t *b, int len) {
    FILE *fd = fopen(path, "wb");
    assert(fd);
    int wlen = fwrite(b, len, 1, fd);
    assert(wlen == 1);
    fclose(fd);
}
void dump_pab_item(PakItem item, FILE *pab_fd) {
    
    uint8_t *data = new uint8_t[item.size];
    int len = fread(data, item.size, 1, pab_fd);
    assert(len == 1);

    //uint8_t *decomp_buff = new uint8_t[item.size * 2];
    //int decomp_len = decompress_lzss(data, item.size, decomp_buff);

    fseek(pab_fd, SEEK_SET, item.offset);

    //printf("decomp: %d - %08x - %d\n", decomp_len, item.offset, item.size);
    //dump_buffer("test.qb", data, item.size);
    //show_dump(decomp_buff, decomp_len, stdout);

    show_dump(data, item.size, stdout);

    int next = (item.offset & 0xFFFFFF00) >> 4;

    printf("offset: %08x\n", next);
    fseek(pab_fd, SEEK_SET,next);

    static int idx = 0;
    if(++idx == 3) {
        exit(-1);
    }
    

    

    //delete[] data;
}
int main(int argc, const char *argv[]) {
    if(argc  < 3) {
        fprintf(stderr, "usage: %s [pak_path] [pab_path]\n",argv[0]);
        return -1;
    }


    FILE *pab_fd = fopen(argv[2], "rb");
    if(!pab_fd) {
        fprintf(stderr, "Failed to open PAB: %s\n", argv[2]);
        return -1;
    }


    FileStream pak_fd(argv[1]);

    if(!pak_fd.IsFileOpened()) {
        fprintf(stderr, "Failed to open PAK: %s\n", argv[1]);
        return -1;
    }

    pak_fd.SetReadEndian(ISTREAM_BIG_ENDIAN);

    PakItem item;
    while(true) {
        item.type = pak_fd.ReadUInt32();
        item.offset = pak_fd.ReadUInt32();
        item.size = pak_fd.ReadUInt32();
        item.unk2 = pak_fd.ReadUInt32();
        item.name = pak_fd.ReadUInt32();
        item.short_name = pak_fd.ReadUInt32();
        item.unk[0] = pak_fd.ReadUInt32();
        item.unk[1] = pak_fd.ReadUInt32();

  
        dump_pab_item(item, pab_fd);
    }
    return 0;
}