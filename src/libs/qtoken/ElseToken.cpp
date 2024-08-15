#include "ElseToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

ElseToken::ElseToken() {

}
ElseToken::~ElseToken() {

}
EScriptToken ElseToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_ELSE;
}
void ElseToken::LoadParams(IStream *stream) {
    m_file_offset = stream->GetOffset() - sizeof(uint8_t);
}
std::string ElseToken::ToString() {
    return "else";
}