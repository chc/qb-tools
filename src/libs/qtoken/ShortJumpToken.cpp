#include "ShortJumpToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>


ShortJumpToken::ShortJumpToken() {

}
ShortJumpToken::~ShortJumpToken() {

}
EScriptToken ShortJumpToken::GetType() {
    return ESCRIPTTOKEN_SHORTJUMP;
}
void ShortJumpToken::LoadParams(IStream *stream) {
    m_file_offset = stream->GetOffset() - 1;
    m_offset = stream->ReadUInt16();

}
void ShortJumpToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_SHORTJUMP);
    stream->WriteUInt16(m_offset);
}
void ShortJumpToken::RewriteOffset(IStream *stream, size_t diff) {
    size_t cursor = stream->GetOffset();

    stream->SetCursor(m_file_offset+1);
    stream->WriteUInt16(diff);

    stream->SetCursor(cursor);
}
std::string ShortJumpToken::ToString() {
    return "";// "sjmp ";
}