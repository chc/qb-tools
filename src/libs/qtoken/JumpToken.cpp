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
    m_file_offset = stream->GetOffset() - 1;
    
    m_offset = stream->ReadUInt32();
}
void JumpToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_JUMP);
    stream->WriteUInt32(m_offset);
}
void JumpToken::RewriteOffset(IStream *stream, size_t diff) {
    size_t cursor = stream->GetOffset();

    stream->SetCursor(m_file_offset+1);
    stream->WriteUInt32(diff);

    stream->SetCursor(cursor);
}
std::string JumpToken::ToString() {
    /*std::ostringstream ss;
    ss << "JMP(" << m_offset << ")";
    return ss.str();*/
    return "";
}