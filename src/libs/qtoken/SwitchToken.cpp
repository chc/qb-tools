#include "SwitchToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

SwitchToken::SwitchToken() {

}
SwitchToken::~SwitchToken() {

}
EScriptToken SwitchToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_SWITCH;
}
void SwitchToken::LoadParams(IStream *stream) {
    m_file_offset = stream->GetOffset() - sizeof(uint8_t);
}
std::string SwitchToken::ToString() {
    return AppendSpaceToString ? "switch " : "switch";
}