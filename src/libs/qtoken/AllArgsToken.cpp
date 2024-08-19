#include "AllArgsToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

AllArgsToken::AllArgsToken() {

}
AllArgsToken::~AllArgsToken() {

}
EScriptToken AllArgsToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_ALLARGS;
}
void AllArgsToken::LoadParams(IStream *stream) {
    m_file_offset = stream->GetOffset() - sizeof(uint8_t);
}
std::string AllArgsToken::ToString() {
    return AppendSpaceToString ? "<...> " : "<...>";
}