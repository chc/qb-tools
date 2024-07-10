#include "StringToken.h"
#include "../IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

StringToken::StringToken() {

}
StringToken::~StringToken() {

}
EScriptToken StringToken::GetType() {
    return ESCRIPTTOKEN_STRING;
}
void StringToken::LoadParams(IQStream *stream) {
    uint32_t length = stream->ReadUInt32();
    while(length--) {
        char ch = stream->ReadByte();
        if(ch == 0)
            break;
        m_value += ch;
    }
    
}
std::string StringToken::ToString() {
    std::ostringstream ss;
    ss << "\"" << m_value << "\" ";
    return ss.str();
}