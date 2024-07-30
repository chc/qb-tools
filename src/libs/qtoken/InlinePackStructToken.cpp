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

extern int32_t g_last_script_keyword;
extern int32_t g_last_script_keyword_write;

void WriteStructure(StructureSymbol *symbol, IStream *stream, bool packed);

InlinePackStructToken::InlinePackStructToken() {

}
InlinePackStructToken::~InlinePackStructToken() {

}
EScriptToken InlinePackStructToken::GetType() {
    return ESCRIPTTOKEN_INLINEPACKSTRUCT;
}

#include <string.h>
#include <stdio.h>



void show_dump(unsigned char *data, unsigned int len, FILE *stream) {
    static const char       hex[] = "0123456789abcdef";
    static unsigned char    buff[67];   /* HEX  CHAR\n */
    unsigned char           chr,
                            *bytes,
                            *p,
                            *limit,
                            *glimit = data + len;

    memset(buff + 2, ' ', 48);

    while(data < glimit) {
        limit = data + 16;
        if(limit > glimit) {
            limit = glimit;
            memset(buff, ' ', 48);
        }

        p     = buff;
        bytes = p + 50;
        while(data < limit) {
            chr = *data;
            *p++ = hex[chr >> 4];
            *p++ = hex[chr & 15];
            p++;
            *bytes++ = ((chr < ' ') || (chr >= 0x7f)) ? '.' : chr;
            data++;
        }
        *bytes++ = '\n';

        fwrite(buff, bytes - buff, 1, stream);
    }
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
        show_dump(buff, len, stdout);
        ms.SetReadEndian(ISTREAM_BIG_ENDIAN);

        m_inner_struct.LoadParamsNoOffset(&ms);

        delete[] buff;
    }
}
void InlinePackStructToken::Write(IStream *stream) {

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