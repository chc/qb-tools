#include "WideStringToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

#include <cassert>

#include <iostream>
#include <codecvt>

WideStringToken::WideStringToken() {

}
WideStringToken::~WideStringToken() {

}
EScriptToken WideStringToken::GetType() {
    return ESCRIPTTOKEN_WIDESTRING;
}
void WideStringToken::LoadParams(IStream *stream) {
    m_file_offset = stream->GetOffset();
    uint32_t len = stream->ReadUInt32();

    uint8_t mode = stream->GetReadEndianMode();

    //XXX: update this to read platform endian mode
    stream->SetReadEndian(ISTREAM_BIG_ENDIAN);


    while(len) {
        uint16_t c = stream->ReadUInt16();
        if(c != 0) {
            m_value += c;
        }
        
        len -= 2;
    }

    stream->SetReadEndian(mode);
}
void WideStringToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    
    stream->WriteByte(ESCRIPTTOKEN_WIDESTRING);
    uint32_t len = m_value.length();
    len++; //include null byte
    stream->WriteUInt32(len * 2);

    uint8_t mode = stream->GetWriteEndianMode();

    //XXX: update this to read platform endian mode
    stream->SetWriteEndian(ISTREAM_BIG_ENDIAN);

    for(int i=0;i<len;i++) {
        stream->WriteUInt16(m_value[i]);
    }
    stream->SetWriteEndian(mode);
}
std::string WideStringToken::ToString() {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    std::string v = converter.to_bytes(m_value);

    std::ostringstream ss;
    ss << "$'" << v << "'" << (AppendSpaceToString ? " " : "");
    return ss.str();
}