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
}
void FastIfToken::Write(IStream *stream) {
    stream->WriteByte(ESCRIPTTOKEN_KEYWORD_FASTIF);
    stream->WriteUInt16(m_offset);
}
std::string FastIfToken::ToString() {
    return "if2 ";
}
int FastIfToken::GetPostTabOffset() {
    return 1;
}