#include "QScriptSymbol.h"

#include <sstream>
#include <iomanip>
#include <cassert>

#include "crc32.h"
#include "lzss.h"

//#define NO_LZSS_COMPRESS


QScriptSymbol::QScriptSymbol() {
    m_decomp_buff = NULL;
    m_decomp_len = 0;

}
QScriptSymbol::QScriptSymbol(uint8_t *decomp_buff, uint32_t decomp_len) : m_decomp_buff(decomp_buff), m_decomp_len(decomp_len) {

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


        uint32_t checksum = crc32(0, (void *)m_decomp_buff, uncompressed_size);
        delete[] comp_buff;
    }

    stream->Align();
}

void QScriptSymbol::Write(IStream *stream) {
    stream->WriteUInt32( stream->GetOffset() + sizeof(uint32_t) + sizeof(uint32_t));
    stream->WriteUInt32(0);

    uint32_t checksum = crc32(0, m_decomp_buff, m_decomp_len);
    stream->WriteUInt32(checksum);

    stream->WriteUInt32(m_decomp_len);

    uint8_t *comp_buff = new uint8_t[m_decomp_len];
    int comp_len = compress_lzss(m_decomp_buff, m_decomp_len, comp_buff);
    #ifdef NO_LZSS_COMPRESS
        comp_len = m_decomp_len;
    #endif
    if(comp_len < m_decomp_len) { //compression was smaller, write
        stream->WriteUInt32(comp_len);
        stream->WriteBuffer(comp_buff, comp_len);
    } else {
        stream->WriteUInt32(m_decomp_len);
        stream->WriteBuffer(m_decomp_buff, m_decomp_len);
    }
    stream->WriteAlign();
}
void QScriptSymbol::WriteToArray(IStream *stream) {
    assert(0);
}
std::string QScriptSymbol::ToString() {
    return "";
}