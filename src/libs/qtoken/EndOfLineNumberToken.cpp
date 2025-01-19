#include "EndOfLineNumberToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <iostream>
EndOfLineNumberToken::EndOfLineNumberToken() : EndOfLineNumberToken(0) {

}
EndOfLineNumberToken::EndOfLineNumberToken(uint32_t line_number) {
    m_line_number = line_number;
}
EndOfLineNumberToken::~EndOfLineNumberToken() {

}
EScriptToken EndOfLineNumberToken::GetType() {
    return ESCRIPTTOKEN_ENDOFLINENUMBER;
}
void EndOfLineNumberToken::Write(IStream* stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_ENDOFLINENUMBER);
    stream->WriteUInt32(m_line_number);
}
void EndOfLineNumberToken::LoadParams(IStream* stream) {
    m_file_offset = stream->GetOffset() - sizeof(uint8_t);
    m_line_number = stream->ReadUInt32();
}
uint32_t EndOfLineNumberToken::GetLineNumber() {
    return m_line_number;
}
std::string EndOfLineNumberToken::ToString() {
    return "\n";
}