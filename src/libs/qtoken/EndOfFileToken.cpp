#include "EndOfFileToken.h"
#include "IQStream.h"

#include <stdio.h>
EndOfFileToken::EndOfFileToken() {

}
EndOfFileToken::~EndOfFileToken() {

}
EScriptToken EndOfFileToken::GetType() {
    return ESCRIPTTOKEN_ENDOFFILE;
}
void EndOfFileToken::LoadParams(IStream *stream) {
    m_file_offset = stream->GetOffset() - sizeof(uint8_t);
}
std::string EndOfFileToken::ToString() {
    return "\n";
}