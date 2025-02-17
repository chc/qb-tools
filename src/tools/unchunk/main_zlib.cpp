
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
    while(read_fd.ReadUInt32() == 1128812107) {
        uint32_t file_offset = read_fd.GetOffset() - sizeof(uint32_t);
        
        uint32_t header_size = read_fd.ReadUInt32();
        uint32_t chunk_size = read_fd.ReadUInt32();
        uint32_t next_offset = read_fd.ReadUInt32();
        uint32_t next_size = read_fd.ReadUInt32();
        uint32_t decomp_size = read_fd.ReadUInt32();
        uint32_t total_decomp = read_fd.ReadUInt32(); //this is basically where to write it to in memory for the game
        printf("Chunk at: %08x\nHeader Size: %d, compressed chunk size: %08x, next at: %08x\n", file_offset, header_size, chunk_size, file_offset+next_offset);
        printf("Decompress size: %d, write mem address: %p, next total size: %d\n", decomp_size ,total_decomp, next_size);
        read_fd.SetCursor(file_offset + header_size);

        assert(((file_offset + 0x7FF) & ~0x7FF) == file_offset);

        //uint8_t *comp_buff = new uint8_t[decomp_size];
        //uint8_t *decomp_buff = new uint8_t[decomp_size];
        //printf("avail in:%d\n", strm.avail_in);

        if(ferror(read_fd.GetHandle())) {
            fprintf(stderr, "chunk read error\n");
            return -1;
        }


        int decomp_written = 0;

        int ret;
        do {
            strm.next_in = in_buff;
            int read_len = CHUNK_SIZE;
            if(decomp_size < CHUNK_SIZE) {
                read_len = decomp_size;
            }
            strm.avail_in = fread(strm.next_in, 1, read_len, read_fd.GetHandle());
            if(strm.avail_in <= 0) {
                break;
            }

            do {
                
                strm.avail_out = CHUNK_SIZE;
                strm.next_out = out_buff;
                inflateReset(&strm);
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
                decomp_size -= read_len;
                    
                if(ret == Z_STREAM_END) {
                    break;   
                }
                else {
                    fprintf(stderr, "Unfinished stream found in CHNK buffer!\n");
                    assert(false);
                }


            } while(strm.avail_out == 0);        
            break;
        } while(decomp_size > 0);

        printf("decomp written: %d / %08x\n\n", decomp_written, decomp_written);
    
        inflateReset2(&strm, -MAX_WBITS);

        if (next_offset == 0xffffffff) {
            break;
        }

        read_fd.SetCursor(file_offset + next_offset);

    }
    inflateEnd(&strm);
    delete[] in_buff;
    delete[] out_buff;

    fclose(out_fd);


    //verify ending alignment
    FILE* fd = read_fd.GetHandle();
    fseek(fd, 0, SEEK_END);
	size_t size = ftell(fd);
    assert(((size + 0xFFF) & ~0xFFF) == size);

}