#include "LocalStringToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

LocalStringToken::LocalStringToken() {

}
LocalStringToken::LocalStringToken(std::string v) : m_value(v) {

}
LocalStringToken::~LocalStringToken() {

}
EScriptToken LocalStringToken::GetType() {
    return ESCRIPTTOKEN_LOCALSTRING;
}
void LocalStringToken::LoadParams(IStream *stream) {
    uint32_t length = stream->ReadUInt32();
    while(length--) {
        char ch = stream->ReadByte();
        if(ch == 0)
            break;
        m_value += ch;
    }   
}
void LocalStringToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_LOCALSTRING);
    stream->WriteUInt32(m_value.length()+1);
    stream->WriteNTS(m_value.c_str());
}
std::string LocalStringToken::ToString() {
    std::ostringstream ss;
    
    // fix this to place backslash before ' and before backslash
    
    ss << "'" << m_value << "'" << (AppendSpaceToString ? " " : "");
    return ss.str();
}