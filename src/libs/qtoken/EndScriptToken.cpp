#include "EndScriptToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

EndScriptToken::EndScriptToken() {

}
EndScriptToken::~EndScriptToken() {

}
EScriptToken EndScriptToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_ENDSCRIPT;
}
void EndScriptToken::LoadParams(IStream *stream) {
    m_file_offset = stream->GetOffset() - sizeof(uint8_t);
}
std::string EndScriptToken::ToString() {
    return "endscript";
}