#include "EndOfLineToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <iostream>
EndOfLineToken::EndOfLineToken() {

}
EndOfLineToken::~EndOfLineToken() {

}
EScriptToken EndOfLineToken::GetType() {
    return ESCRIPTTOKEN_ENDOFLINE;
}
void EndOfLineToken::LoadParams(IStream *stream) {
}
void EndOfLineToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_ENDOFLINE);
}
std::string EndOfLineToken::ToString() {
    return "\n";
}