#include "IntegerToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

IntegerToken::IntegerToken() {

}
IntegerToken::IntegerToken(int32_t v) {
    m_value = v;
}
IntegerToken::~IntegerToken() {

}
EScriptToken IntegerToken::GetType() {
    return ESCRIPTTOKEN_INTEGER;
}
void IntegerToken::LoadParams(IStream *stream) {
    m_value = stream->ReadInt32();
}
void IntegerToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_INTEGER);
    stream->WriteInt32(m_value);
}
std::string IntegerToken::ToString() {
    std::ostringstream ss;
    ss << m_value << " ";
    return ss.str();
}