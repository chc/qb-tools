#include "QScriptSymbol.h"

#include <sstream>
#include <iomanip>
#include <cassert>

#include "../crc32.h"
#include "../lzss.h"
#include "../show_dump.h"


QScriptSymbol::QScriptSymbol() {

}
QScriptSymbol::~QScriptSymbol() {

}
ESymbolType QScriptSymbol::GetType() {
    return ESYMBOLTYPE_QSCRIPT;
}
void QScriptSymbol::LoadParams(IStream *stream) {
    
    uint32_t size  = stream->ReadInt32();
    stream->ReadInt32(); //??

    printf("QScriptSymbol data_checksum at: %p\n", stream->GetOffset());
    uint32_t data_checksum = stream->ReadUInt32();
    uint32_t uncompressed_size = stream->ReadUInt32();
    uint32_t compressed_size = stream->ReadUInt32();

    //printf("data checksum: %d\n", data_checksum);
    //printf("uncompressed_size: %d\n", uncompressed_size);
    //printf("compressed_size: %d\n", compressed_size);

    uint8_t *decomp_buff = new uint8_t[uncompressed_size];
    uint8_t *comp_buff = new uint8_t[compressed_size];


    //do this better..
    for(int i=0;i<compressed_size;i++) {
        comp_buff[i] = stream->ReadByte();
    }
    //stream->ReadByte();
    //stream->ReadByte();
    //stream->ReadByte();
    //stream->ReadByte();

    while((stream->GetOffset() & 4)) {
        //printf("skipping: %d\n", stream->GetOffset());
        stream->ReadByte();
    }

    //DecodeLZSS((unsigned char *)comp_buff, (unsigned char *)decomp_buff, compressed_size);

    int decomp_len = decompress_lzss((unsigned char *)comp_buff, compressed_size, (unsigned char *)decomp_buff);

    uint32_t checksum = crc32(0, (void *)decomp_buff, uncompressed_size);

    //printf("QScript data: \n");
    //show_dump((unsigned char *)decomp_buff, decomp_len, stdout);

    //printf("checksum: %08x == %08x, decomp: %d\n", data_checksum, checksum, decomp_len);
    //assert(checksum == data_checksum);

    delete[] decomp_buff;
    delete[] comp_buff;
}
std::string QScriptSymbol::ToString() {
    return "";
}