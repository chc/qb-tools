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

}
InlinePackStructToken::~InlinePackStructToken() {

}
EScriptToken InlinePackStructToken::GetType() {
    return ESCRIPTTOKEN_INLINEPACKSTRUCT;
}

void InlinePackStructToken::LoadParams(IStream *stream) {
    uint16_t len = stream->ReadUInt16();    

    int offset = stream->GetOffset() - g_last_script_keyword;
    offset += 5;
    offset -= 2; //len
    
    while((offset % 4)) {
        offset++;
        stream->ReadByte();
    }

    if(len > 0) {
        uint8_t *buff = new uint8_t[len];
        for(int i=0;i<len;i++) {
            buff[i] = stream->ReadByte();
        }

        MemoryStream ms(buff, len);
        ms.SetReadEndian(ISTREAM_BIG_ENDIAN);

        m_inner_struct.LoadParamsNoOffset(&ms);

        delete[] buff;
    }
}
void InlinePackStructToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();

    //don't write the actual pack struct data, the qdumper will dump the inner struct (this should be refactored)
    stream->WriteByte(ESCRIPTTOKEN_INLINEPACKSTRUCT);
    stream->WriteUInt16(0);

    int offset = stream->GetOffset() - g_last_script_keyword_write;
    while((offset % 4)) {
        offset++;
        stream->WriteByte(0);
    }

}
std::string InlinePackStructToken::ToString() {
    return "*";
}