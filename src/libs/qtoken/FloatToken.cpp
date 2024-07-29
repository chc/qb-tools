#include "FloatToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

FloatToken::FloatToken() {

}
FloatToken::FloatToken(float v) {
    m_value = v;
}
FloatToken::~FloatToken() {

}
EScriptToken FloatToken::GetType() {
    return ESCRIPTTOKEN_FLOAT;
}
void FloatToken::LoadParams(IStream *stream) {
    m_value = stream->ReadFloat();
}
void FloatToken::Write(IStream *stream) {
    stream->WriteByte(ESCRIPTTOKEN_FLOAT);
    stream->WriteFloat(m_value);
}
std::string FloatToken::ToString() {
    std::ostringstream ss;
    ss << std::setprecision(2);
    ss << std::fixed;
    ss << m_value << " ";
    return ss.str();
}