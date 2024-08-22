#include "StringQSToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

StringQSToken::StringQSToken() {

}
StringQSToken::~StringQSToken() {

}
EScriptToken StringQSToken::GetType() {
    return ESCRIPTTOKEN_STRINGQS;
}
void StringQSToken::LoadParams(IStream *stream) {
    m_file_offset = stream->GetOffset() - sizeof(uint8_t);
    m_checksum = stream->ReadUInt32();
}
void StringQSToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_STRINGQS);
    stream->WriteUInt32(m_checksum);
}
std::string StringQSToken::ToString() {
    return "SQS";
}