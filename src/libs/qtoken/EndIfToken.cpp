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
    printf("Load EndIf: %08x\n", stream->GetOffset());
}
void EndIfToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_KEYWORD_ENDIF);
    printf("** Write EndIf: %08x\n", stream->GetOffset());
}
std::string EndIfToken::ToString() {
    return "endif";
}
int EndIfToken::GetPreTabOffset() {
    return -1;
}