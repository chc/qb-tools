#include "EndOfLineNumberToken.h"
#include "../IQStream.h"

#include <stdio.h>
#include <iostream>
EndOfLineNumberToken::EndOfLineNumberToken() {

}
EndOfLineNumberToken::~EndOfLineNumberToken() {

}
EScriptToken EndOfLineNumberToken::GetType() {
    return ESCRIPTTOKEN_ENDOFLINENUMBER;
}
void EndOfLineNumberToken::LoadParams(IQStream *stream) {
    m_line_number = stream->ReadUInt32();
}
uint32_t EndOfLineNumberToken::GetLineNumber() {
    return m_line_number;
}
std::string EndOfLineNumberToken::ToString() {
    return "\n";
}