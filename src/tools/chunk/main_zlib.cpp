
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cassert>
#include <zlib.h>
#include <FileStream.h>
#include <stack>

const size_t HEADER_SIZE = 0x80;

typedef struct {
    size_t header_address;
    size_t buffer_address;
    uint32_t buffer_size;
    size_t absolute_end;
    size_t writeto_address;
    size_t decomp_size;
} ChunkInfo;
std::stack<ChunkInfo> chunks;

void append_chunk(z_stream* strm, FileStream& out_fd);

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

    int ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, -MAX_WBITS, MAX_MEM_LEVEL, Z_HUFFMAN_ONLY);
    if (ret != Z_OK) {
        fprintf(stderr, "Failed to init zlib: %d\n", ret);
        return ret;
    }

    while (!feof(in_fd)) {
        uint32_t len = fread(in_buff, 1, BUFFER_CHUNK_SIZE, in_fd);

        deflateReset(&strm);
        strm.avail_in = len;
        strm.next_in = in_buff;

        append_chunk(&strm, out_fd);
    }

    //pad end of file
    size_t end_address = out_fd.GetOffset();

    size_t new_diff = (end_address + 0xFFF) & ~0xFFF;
    new_diff -= end_address;
    while (new_diff--) {
        out_fd.WriteByte(0x00);
    }



    //update header entries
    uint32_t last_address = 0xffffffff, last_size = 0;


    while (!chunks.empty()) {
		ChunkInfo chunk = chunks.top();
        chunks.pop();
        out_fd.SetCursor(chunk.header_address);

        out_fd.WriteUInt32(1128812107);
        out_fd.WriteUInt32(HEADER_SIZE);
        out_fd.WriteUInt32(chunk.buffer_size);

        if (last_address != 0xffffffff) {
            uint32_t next_offset = last_address - chunk.header_address; //last_address;
            out_fd.WriteUInt32(next_offset);
        }
        else {
            out_fd.WriteUInt32(0xffffffff);
        }
        out_fd.WriteUInt32(last_size);


        out_fd.WriteUInt32(chunk.decomp_size); //decomp size
        out_fd.WriteUInt32(chunk.writeto_address);

		size_t total_size = chunk.absolute_end - chunk.header_address;

        last_size = total_size;
        last_address = chunk.header_address;

    }


    delete[] in_buff;
    delete[] out_buff;
    return 0;
}


void append_chunk(z_stream *strm, FileStream& out_fd) {
    ChunkInfo chunk;
    size_t buffer_size = 0;

    chunk.header_address = out_fd.GetOffset();
	chunk.buffer_address = chunk.header_address + HEADER_SIZE;
    chunk.decomp_size = strm->avail_in;

    //write dummy values for now
    out_fd.WriteUInt32(0xAAAAAAAA); //header
    out_fd.WriteUInt32(0xBBBBBBBB); //offset;
    out_fd.WriteUInt32(0xCCCCCCCC); //chunk size
    out_fd.WriteUInt32(0xDDDDDDDD); //next
    out_fd.WriteUInt32(0xEEEEEEEE); //unk
    out_fd.WriteUInt32(0xFFFFFFFF); //size 2
    out_fd.WriteUInt32(0x22222222); //out_offset

    out_fd.SetCursor(chunk.buffer_address);

    do {
        strm->avail_out = WRITE_CHUNK_SIZE;
        strm->next_out = out_buff;

        int ret = deflate(strm, Z_FINISH);
        assert(ret != Z_STREAM_ERROR);

        int have = WRITE_CHUNK_SIZE - strm->avail_out;
        if (fwrite(out_buff, 1, have, out_fd.GetHandle()) != have || ferror(out_fd.GetHandle())) {
            (void)deflateEnd(strm);
            assert(false);
            return;
        }
        buffer_size += have;
    } while (strm->avail_out == 0);

    size_t end_address = out_fd.GetOffset();


    size_t new_diff = (end_address + 0x7FF) & ~0x7FF;
    new_diff -= end_address;
    while (new_diff--) {
        out_fd.WriteByte(0x00);
    }



    chunk.buffer_size = buffer_size;
	chunk.absolute_end = end_address;
    chunk.writeto_address = absolute_total_decomp;
    absolute_total_decomp += chunk.decomp_size;

    chunks.push(chunk);

}