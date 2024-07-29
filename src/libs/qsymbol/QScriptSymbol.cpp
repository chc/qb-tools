#include "QScriptSymbol.h"

#include <sstream>
#include <iomanip>
#include <cassert>

#include "crc32.h"
#include "lzss.h"


QScriptSymbol::QScriptSymbol() {
    m_decomp_buff = NULL;
    m_decomp_len = 0;

}
QScriptSymbol::~QScriptSymbol() {
    if(m_decomp_buff != NULL) {
        delete[] m_decomp_buff;
    }

}
ESymbolType QScriptSymbol::GetType() {
    return ESYMBOLTYPE_QSCRIPT;
}
void QScriptSymbol::LoadParams(IStream *stream) {
    
    uint32_t size  = stream->ReadInt32();
    stream->ReadInt32(); //??

    uint32_t data_checksum = stream->ReadUInt32();
    uint32_t uncompressed_size = stream->ReadUInt32();
    uint32_t compressed_size = stream->ReadUInt32();

    //printf("data checksum: %08x\n", data_checksum);
    //printf("uncompressed_size: %d\n", uncompressed_size);
    //printf("compressed_size: %d\n", compressed_size);

    m_decomp_buff = new uint8_t[uncompressed_size];
    uint8_t *comp_buff = new uint8_t[compressed_size];


    //do this better..
    for(int i=0;i<compressed_size;i++) {
        comp_buff[i] = stream->ReadByte();
    }

    stream->Align();

    //DecodeLZSS((unsigned char *)comp_buff, (unsigned char *)decomp_buff, compressed_size);

    m_decomp_len = decompress_lzss((unsigned char *)comp_buff, compressed_size, (unsigned char *)m_decomp_buff);

    uint32_t checksum = crc32(0, (void *)m_decomp_buff, uncompressed_size);

    printf("checksum: %08x == %08x, decomp: %d\n", data_checksum, checksum, m_decomp_len);
    //assert(checksum == data_checksum);

    delete[] comp_buff;
}
std::string QScriptSymbol::ToString() {
    return "";
}