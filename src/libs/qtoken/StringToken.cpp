#include "StringToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

StringToken::StringToken() {

}
StringToken::StringToken(std::string v) : m_value(v) {
    
}
StringToken::~StringToken() {

}
EScriptToken StringToken::GetType() {
    return ESCRIPTTOKEN_STRING;
}
void StringToken::LoadParams(IStream *stream) {
    uint32_t length = stream->ReadUInt32();
    while(length--) {
        char ch = stream->ReadByte();
        if(ch == 0)
            break;
        m_value += ch;
    }
}
void StringToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_STRING);
    stream->WriteUInt32(m_value.length()+1);
    stream->WriteNTS(m_value.c_str());
}
std::string StringToken::ToString() {
    std::ostringstream ss;
    ss << "\"" << m_value << "\" ";
    return ss.str();
}