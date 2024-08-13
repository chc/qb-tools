#include "EndIfToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

EndIfToken::EndIfToken() {

}
EndIfToken::~EndIfToken() {

}
EScriptToken EndIfToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_ENDIF;
}
void EndIfToken::LoadParams(IStream *stream) {
}
void EndIfToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_KEYWORD_ENDIF);
}
std::string EndIfToken::ToString() {
    return "endif";
}