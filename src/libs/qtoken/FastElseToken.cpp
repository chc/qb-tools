#include "FastElseToken.h"
#include "IQStream.h"

#include <stdio.h>
FastElseToken::FastElseToken() {

}
EScriptToken FastElseToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_FASTELSE;
}
void FastElseToken::LoadParams(IStream *stream) {
    m_offset = stream->ReadUInt16();
}
void FastElseToken::Write(IStream *stream) {
    stream->WriteByte(ESCRIPTTOKEN_KEYWORD_FASTELSE);
    stream->WriteUInt16(m_offset);
}
std::string FastElseToken::ToString() {
    return "else2 ";
}
int FastElseToken::GetPreTabOffset() {
    return -1;
}
int FastElseToken::GetPostTabOffset() {
    return 1;
}
std::vector<TokenInjection> FastElseToken::GetInjections() {
    TokenInjection i;
    i.use_next_jump_offset = false;
    i.offset = m_offset - 2;
    i.token = "**END ELSE2**";
    std::vector<TokenInjection> v;
    v.push_back(i);
    return v;
}