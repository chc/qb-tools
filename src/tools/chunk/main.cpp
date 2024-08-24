
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cassert>
#include <zlib.h>
#include <FileStream.h>

#define CHUNK_SIZE (0x80000)
int main(int argc, const char* argv[]) {
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

    uint8_t in_buff[CHUNK_SIZE], out_buff[CHUNK_SIZE];
    int absolute_total_decomp = 0;
    size_t previous_file, first_next = 0;
    do {
        int total_comp_size = 0;
        int total_decomp_size = 0;

        /* allocate inflate state */
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = 0;
        strm.next_in = Z_NULL;
        //
        int ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -MAX_WBITS, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY);
        if (ret != Z_OK) {
            fprintf(stderr, "Failed to init zlib: %d\n", ret);
            return ret;
        }

        strm.avail_in = fread(&in_buff[0], 1, CHUNK_SIZE, in_fd);
        strm.next_in = &in_buff[0];

        if(ferror(in_fd)) {
            fprintf(stderr, "File error reading input buffer\n");
            return -1;
        }

        if (strm.avail_in == 0)
            break;

        total_decomp_size += strm.avail_in;



        int flush = feof(in_fd) ? Z_FINISH : Z_SYNC_FLUSH;
        uint32_t file_offset = out_fd.GetOffset();
        out_fd.WriteUInt32(1128812107);
        out_fd.WriteUInt32(0); //offset;
        out_fd.WriteUInt32(0); //chunk size
        out_fd.WriteUInt32(0); //next
        out_fd.WriteUInt32(0); //unk
        out_fd.WriteUInt32(0); //size 2
        out_fd.WriteUInt32(0); //out_offset

        out_fd.WriteAlign();
        out_fd.SetCursor(out_fd.GetOffset() + 0x64);

        size_t prewrite_offset = out_fd.GetOffset() - file_offset;


        do {
            strm.avail_out = CHUNK_SIZE;
            strm.next_out = &out_buff[0];
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            int have = CHUNK_SIZE - strm.avail_out;
            total_comp_size += have;
            if (fwrite(&out_buff[0], 1, have, out_fd.GetHandle()) != have || ferror(out_fd.GetHandle())) {
                (void)deflateEnd(&strm);
                return Z_ERRNO;
            }

        } while(strm.avail_out == 0);
        deflateEnd(&strm);

        size_t new_offset = out_fd.GetOffset();
        new_offset += 0x540;
        while(new_offset % 0x10) {
            new_offset++;
        }
        out_fd.SetCursor(new_offset);
        
        size_t done_offset = out_fd.GetOffset();
        out_fd.SetCursor(file_offset + sizeof(uint32_t)); 
        out_fd.WriteUInt32(prewrite_offset); //offset
        out_fd.WriteUInt32(total_comp_size); //comp_size
        assert(total_comp_size != 0);

        uint32_t next_value = feof(in_fd) ? 0xffffffff : (done_offset - file_offset);
        out_fd.WriteUInt32(next_value); //next offset
        out_fd.WriteUInt32(0);  //unk
        out_fd.WriteUInt32(total_decomp_size); //total comp size?
        out_fd.WriteUInt32(absolute_total_decomp); //

        absolute_total_decomp += total_decomp_size;

        out_fd.SetCursor(previous_file + (sizeof(uint32_t)*4));

        if(first_next == 0) {
            first_next = next_value;
        }
        if(next_value == 0xffffffff) {
            next_value = first_next;
        }
        out_fd.WriteUInt32(next_value);
        
        out_fd.SetCursor(done_offset);
        previous_file = file_offset;
    } while(true);

    size_t new_offset = out_fd.GetOffset();
    new_offset += 0x540;
    while(new_offset % 0x10) {
        new_offset++;
    }
    out_fd.SetCursor(new_offset-sizeof(uint8_t));
    out_fd.WriteByte(0);
    return 0;
}