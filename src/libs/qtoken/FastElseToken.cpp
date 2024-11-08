#include "FastElseToken.h"
#include "IQStream.h"

#include <stdio.h>
FastElseToken::FastElseToken() {

}
EScriptToken FastElseToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_FASTELSE;
}
void FastElseToken::LoadParams(IStream *stream) {
    m_file_offset = stream->GetOffset() - 1;
    m_offset = stream->ReadUInt16();
}
void FastElseToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_KEYWORD_FASTELSE);
    
    stream->WriteUInt16(0xFFFF);
}
void FastElseToken::RewriteOffset(IStream *stream, size_t diff) {
    size_t cursor = stream->GetOffset();

    stream->SetCursor(m_file_offset+1);
    stream->WriteUInt16(diff);

    stream->SetCursor(cursor);
}
std::string FastElseToken::ToString() {
    return "else";
}
int FastElseToken::GetPreTabOffset() {
    return -1;
}
int FastElseToken::GetPostTabOffset() {
    return 1;
}
std::vector<TokenInjection> FastElseToken::GetInjections() {
    std::vector<TokenInjection> v;
    /*TokenInjection i;
    i.use_next_jump_offset = false;
    i.offset = m_offset - 2;
    i.token = "**END ELSE2**";    
    v.push_back(i);*/
    return v;
}