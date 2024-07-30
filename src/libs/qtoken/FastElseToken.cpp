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