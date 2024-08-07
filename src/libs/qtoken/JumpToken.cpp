#include "JumpToken.h"
#include "IQStream.h"

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
void JumpToken::LoadParams(IStream *stream) {
    m_offset = stream->ReadUInt32();
}
void JumpToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_JUMP);
    stream->WriteUInt32(m_offset);
}
std::string JumpToken::ToString() {
    /*std::ostringstream ss;
    ss << "JMP(" << m_offset << ")";
    return ss.str();*/
    return "";
}
uint32_t JumpToken::GetOffset() {
    return m_offset;
}