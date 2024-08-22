
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cassert>
#include <zlib.h>
#include <FileStream.h>
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




int main(int argc, const char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s [in] [out]\n", argv[0]);
        return -1;
    }

    FileStream read_fd(argv[1]);
    if (!read_fd.IsFileOpened()) {
        fprintf(stderr, "Failed to open input: %s\n", argv[1]);
        return -1;
    }

    FILE *out_fd = fopen(argv[2], "wb");
    if(!out_fd) {
        fprintf(stderr, "Failed to open output: %s\n", argv[1]);
        return -1;
    }

    read_fd.SetReadEndian(ISTREAM_PAK_ENDIAN);

    /* allocate inflate state */
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    int ret = inflateInit2(&strm, -MAX_WBITS);
    if (ret != Z_OK)
        return ret;

    while(read_fd.ReadUInt32() == 1128812107) {
        uint32_t file_offset = read_fd.GetOffset() - sizeof(uint32_t);
        
        uint32_t offset = read_fd.ReadUInt32();
        uint32_t decomp_size = read_fd.ReadUInt32();
        uint32_t next = read_fd.ReadUInt32();
        uint32_t unk1 = read_fd.ReadUInt32();
        uint32_t size2 = read_fd.ReadUInt32();
        uint32_t out_offset = read_fd.ReadUInt32();
        //printf("item: %08x %d %08x\n", offset, decomp_size, next);
        //printf("item extra: %d %d %08x\n", unk1, size2, out_offset);

        read_fd.SetCursor(file_offset + offset);

        uint8_t *comp_buff = new uint8_t[decomp_size];
        uint8_t *decomp_buff = new uint8_t[decomp_size];
        strm.next_in = comp_buff;
        strm.avail_in =  fread(comp_buff, 1, decomp_size, read_fd.GetHandle());
        //printf("avail in:%d\n", strm.avail_in);

        if(ferror(read_fd.GetHandle())) {
            fprintf(stderr, "chunk read error\n");
            return -1;
        }

        int ret;
        do {
            strm.avail_out = decomp_size;
            strm.next_out = decomp_buff;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);
            int have = decomp_size - strm.avail_out;
            switch(ret) {
                case Z_NEED_DICT:
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    fprintf(stderr, "Got libz error: %d\n", ret);
                    return -1;
            }
            fwrite(decomp_buff, have, 1, out_fd);
                
            if(ret == Z_STREAM_END) {
                break;   
            }
        } while(true);

        inflateReset2(&strm, -MAX_WBITS);
        delete[] decomp_buff;
        delete[] comp_buff;


        read_fd.SetCursor(file_offset + next);
        //printf("next chunk: %08x + %08x = %08x\n", file_offset, next, file_offset + next);

    }
    inflateEnd(&strm);

    fclose(out_fd);

}