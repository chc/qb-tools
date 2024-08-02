#include "ElseIfToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

ElseIfToken::ElseIfToken() {

}
ElseIfToken::~ElseIfToken() {

}
EScriptToken ElseIfToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_ELSEIF;
}
void ElseIfToken::LoadParams(IStream *stream) {
    m_offset = stream->ReadUInt32();
}
void ElseIfToken::Write(IStream *stream) {
    stream->WriteByte(ESCRIPTTOKEN_KEYWORD_ELSEIF);
    stream->WriteUInt32(m_offset);
}
std::string ElseIfToken::ToString() {
    return "elseif";
}
int ElseIfToken::GetPreTabOffset() {
    return -1;
}
int ElseIfToken::GetPostTabOffset() {
    return 1;
}