
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cassert>
#include <zlib.h>
#include <FileStream.h>
#include <stack>


// extern "C" {
//     extern int z_verbose;
//}

#define WRITE_CHUNK_SIZE (0x20000)
#define BUFFER_CHUNK_SIZE  0x80000

uint8_t* in_buff = new uint8_t[BUFFER_CHUNK_SIZE];
uint8_t* out_buff = new uint8_t[WRITE_CHUNK_SIZE];
size_t absolute_total_decomp = 0;

int main(int argc, const char* argv[]) {

    //z_verbose = 1;
    if (argc < 2) {
        fprintf(stderr, "usage: %s [in] [out]\n", argv[0]);
        return -1;
    }
    z_stream strm;

    FILE *in_fd = fopen(argv[1], "rb");
    if(!in_fd) {
        fprintf(stderr, "Failed to open input: %s\n", argv[1]);
        return 0;
    }


    FileStream out_fd(argv[2], true);
    if (!out_fd.IsFileOpened()) {
        fprintf(stderr, "Failed to open output: %s\n", argv[2]);
        return -1;
    }    
    out_fd.SetWriteEndian(ISTREAM_PAK_ENDIAN);


    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;

    int ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -MAX_WBITS, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        fprintf(stderr, "Failed to init zlib: %d\n", ret);
        return ret;
    }

    while (!feof(in_fd)) {
        uint32_t len = fread(in_buff, 1, BUFFER_CHUNK_SIZE, in_fd);
        
        deflateReset(&strm);
        strm.avail_in = len;
        strm.next_in = in_buff;

        do {
            strm.avail_out = WRITE_CHUNK_SIZE;
            strm.next_out = out_buff;

            int ret = deflate(&strm, Z_FINISH);
            assert(ret != Z_STREAM_ERROR);

            int have = WRITE_CHUNK_SIZE - strm.avail_out;
            if (fwrite(out_buff, 1, have, out_fd.GetHandle()) != have || ferror(out_fd.GetHandle())) {
                (void)deflateEnd(&strm);
                assert(false);
                return 1;
            }
        } while (strm.avail_out == 0);            
    }


    delete[] in_buff;
    delete[] out_buff;
    return 0;
}