#include "JumpToken.h"
#include "../IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

JumpToken::JumpToken() {

}
JumpToken::~JumpToken() {

}
EScriptToken JumpToken::GetType() {
    return ESCRIPTTOKEN_JUMP;
}
void JumpToken::LoadParams(IQStream *stream) {
    m_offset = stream->ReadUInt32();
    
}
std::string JumpToken::ToString() {
    /*std::ostringstream ss;
    ss << "JMP(" << m_offset << ")";
    return ss.str();*/
    return "";
}