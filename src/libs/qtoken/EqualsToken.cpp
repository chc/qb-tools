#include "EqualsToken.h"
#include "IQStream.h"

#include <stdio.h>
EqualsToken::EqualsToken() {

}
EqualsToken::~EqualsToken() {

}
EScriptToken EqualsToken::GetType() {
    return ESCRIPTTOKEN_EQUALS;
}
void EqualsToken::LoadParams(IStream *stream) {
}
void EqualsToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_EQUALS);
}
std::string EqualsToken::ToString() {
    return "= ";
}