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

    if(compressed_size >= uncompressed_size) { //no compression
        m_decomp_buff = new uint8_t[uncompressed_size];
        
        //do this better..
        for(int i=0;i<compressed_size;i++) {
            m_decomp_buff[i] = stream->ReadByte();
        }
        m_decomp_len = uncompressed_size;
    } else {
        m_decomp_buff = new uint8_t[uncompressed_size];
        uint8_t *comp_buff = new uint8_t[compressed_size];

        //do this better..
        for(int i=0;i<compressed_size;i++) {
            comp_buff[i] = stream->ReadByte();
        }

        m_decomp_len = decompress_lzss((unsigned char *)comp_buff, compressed_size, (unsigned char *)m_decomp_buff);

        /*char temp[256];
        snprintf(temp, sizeof(temp), "%08x.bin", GetNameChecksum());
        FILE *fd = fopen(temp, "wb");
        fwrite(m_decomp_buff, m_decomp_len, 1, fd);
        fclose(fd);*/

        uint32_t checksum = crc32(0, (void *)m_decomp_buff, uncompressed_size);
        delete[] comp_buff;
    }

    stream->Align();
}
std::string QScriptSymbol::ToString() {
    return "";
}