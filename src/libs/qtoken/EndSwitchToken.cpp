#include "EndSwitchToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

EndSwitchToken::EndSwitchToken() {

}
EndSwitchToken::~EndSwitchToken() {

}
EScriptToken EndSwitchToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_ENDSWITCH;
}
void EndSwitchToken::LoadParams(IStream *stream) {
    m_file_offset = stream->GetOffset() - sizeof(uint8_t);
}
std::string EndSwitchToken::ToString() {
    return "endswitch";
}