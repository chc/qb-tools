
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cassert>
#include <zlib.h>
#include <FileStream.h>
#define CHUNK_SIZE (0x80000)

 //extern "C" {
 //    extern int z_verbose;
 //}


int main(int argc, const char *argv[]) {
    //z_verbose = 1;
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
        fprintf(stderr, "Failed to open output: %s\n", argv[2]);
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
    //uint8_t in_buff[CHUNK_SIZE], out_buff[CHUNK_SIZE];
    uint8_t* in_buff = new uint8_t[CHUNK_SIZE];
    uint8_t* out_buff = new uint8_t[CHUNK_SIZE];

    int decomp_written = 0;

    do {
        strm.next_in = in_buff;
        int read_len = CHUNK_SIZE;
        strm.avail_in = fread(strm.next_in, 1, read_len, read_fd.GetHandle());
        if(strm.avail_in <= 0) {
            break;
        }

        do {
            
            strm.avail_out = CHUNK_SIZE;
            strm.next_out = out_buff;
            ret = inflate(&strm, Z_SYNC_FLUSH);
            assert(ret != Z_STREAM_ERROR);
            int have = CHUNK_SIZE - strm.avail_out;
            switch(ret) {
                case Z_NEED_DICT:
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    fprintf(stderr, "Got libz error: %d\n", ret);
                    return -1;
            }
            if(have == 0) {
                break;
            }

            int len = fwrite((void *)out_buff, have, 1, out_fd);
            assert(len == 1);

            decomp_written += have;
                
            if(ret == Z_STREAM_END) {
                break;   
            }
        } while(strm.avail_out == 0);        
    } while(true);

    fprintf(stderr, "decomp written: %d / %08x\n\n", decomp_written, decomp_written);

    inflateEnd(&strm);
    delete[] in_buff;
    delete[] out_buff;

    fclose(out_fd);
    return 0;
}