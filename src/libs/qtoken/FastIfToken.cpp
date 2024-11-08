#include "FastIfToken.h"
#include "IQStream.h"

#include <stdio.h>
FastIfToken::FastIfToken() {

}
EScriptToken FastIfToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_FASTIF;
}
void FastIfToken::LoadParams(IStream *stream) {
    m_file_offset = stream->GetOffset() - sizeof(uint8_t);
    m_offset = stream->ReadUInt16();
}
void FastIfToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_KEYWORD_FASTIF);
    
    stream->WriteUInt16(0xFFFF);
}
void FastIfToken::RewriteOffset(IStream *stream, size_t diff) {
    size_t cursor = stream->GetOffset();

    stream->SetCursor(m_file_offset+sizeof(uint8_t));
    stream->WriteUInt16(diff);

    stream->SetCursor(cursor);
}
std::string FastIfToken::ToString() {
    return AppendSpaceToString ? "if " : "if";
}
int FastIfToken::GetPostTabOffset() {
    return 1;
}
std::vector<TokenInjection> FastIfToken::GetInjections() {
    std::vector<TokenInjection> v;

    /*TokenInjection i;
    i.use_next_jump_offset = false;
    i.offset = m_offset - 2;
    i.token = "**END IF2**";
    v.push_back(i);*/
    return v;
}