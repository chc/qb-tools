#include "BreakToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

BreakToken::BreakToken() {

}
BreakToken::~BreakToken() {

}
EScriptToken BreakToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_BREAK;
}
void BreakToken::LoadParams(IStream *stream) {
    m_file_offset = stream->GetOffset() - sizeof(uint8_t);
}
std::string BreakToken::ToString() {
    return AppendSpaceToString ? "break " : "break";
}