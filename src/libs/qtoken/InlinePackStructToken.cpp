#include "InlinePackStructToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>
#include <cassert>


#include "NameToken.h"
#include "EqualsToken.h"
#include <StructureSymbol.h>
#include "MemoryStream.h"

extern "C" {
    extern int32_t g_last_script_keyword;
    extern int32_t g_last_script_keyword_write;
}

void WriteStructure(StructureSymbol *symbol, IStream *stream, bool packed);

InlinePackStructToken::InlinePackStructToken() {
    m_inner_struct = NULL;
    m_padding = 0;

}
InlinePackStructToken::~InlinePackStructToken() {

}
EScriptToken InlinePackStructToken::GetType() {
    return ESCRIPTTOKEN_INLINEPACKSTRUCT;
}

void InlinePackStructToken::LoadParams(IStream *stream) {
    assert(m_inner_struct == NULL);
    m_file_offset = stream->GetOffset() - 1;
    uint16_t len = stream->ReadUInt16();    

    //set required alignment
    int padding = m_padding;
    while(padding--) {
        stream->ReadByte();
    }    
    //

    if(len > 0) {
        uint8_t *buff = new uint8_t[len];
        for(int i=0;i<len;i++) {
            buff[i] = stream->ReadByte();
        }

        MemoryStream ms(buff, len);
        ms.SetReadEndian(ISTREAM_BIG_ENDIAN);

        m_inner_struct = new StructureSymbol();
        m_inner_struct->LoadParamsNoOffset(&ms);

        delete[] buff;
    }
}
void InlinePackStructToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    //don't write the actual pack struct data, the qdumper will dump the inner struct (this should be refactored)
    stream->WriteByte(ESCRIPTTOKEN_INLINEPACKSTRUCT);
    uint32_t len = 0;

    const int STRUCT_BUFF_SIZE = 1024;
    uint8_t struct_buff[STRUCT_BUFF_SIZE];

    if(m_inner_struct == NULL) {
        len = 0;
    } else {

        MemoryStream ms(&struct_buff, STRUCT_BUFF_SIZE);
        ms.SetWriteEndian(ISTREAM_BIG_ENDIAN);
        m_inner_struct->WriteNoOffset(&ms);
        len = ms.GetOffset();
    }
    stream->WriteUInt16(len);

    int padding = m_padding;
    while(padding--) {
        stream->WriteByte(0);
    }
    if(len > 0) {
        stream->WriteBuffer(struct_buff, len);
    }
}
void InlinePackStructToken::WriteStructure(IStream* stream, StructureSymbol *symbol) {
    assert(false);
}
void InlinePackStructToken::SetValue(StructureSymbol *sym) {
    m_inner_struct = sym;
}
std::string InlinePackStructToken::ToString() {
    return "$";
}