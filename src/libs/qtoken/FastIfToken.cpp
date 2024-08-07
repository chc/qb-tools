#include "FastIfToken.h"
#include "IQStream.h"

#include <stdio.h>
FastIfToken::FastIfToken() {

}
EScriptToken FastIfToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_FASTIF;
}
void FastIfToken::LoadParams(IStream *stream) {
    m_offset = stream->ReadUInt16();
    printf("FastIf offset: %08x (%08x) = %08x\n", m_offset, stream->GetOffset(), m_offset + stream->GetOffset());
}
void FastIfToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_KEYWORD_FASTIF);
    
    stream->WriteUInt16(0xFFFF);
}
void FastIfToken::RewriteOffset(IStream *stream, size_t diff) {
    size_t cursor = stream->GetOffset();

    stream->SetCursor(m_file_offset+1);
    stream->WriteUInt16(diff);

    stream->SetCursor(cursor);
}
std::string FastIfToken::ToString() {
    return "if2 ";
}
int FastIfToken::GetPostTabOffset() {
    return 1;
}
std::vector<TokenInjection> FastIfToken::GetInjections() {
    TokenInjection i;
    i.use_next_jump_offset = false;
    i.offset = m_offset - 2;
    i.token = "**END IF2**";
    std::vector<TokenInjection> v;
    v.push_back(i);
    return v;
}