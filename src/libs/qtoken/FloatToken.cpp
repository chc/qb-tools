#include "FloatToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

FloatToken::FloatToken() {

}
FloatToken::~FloatToken() {

}
EScriptToken FloatToken::GetType() {
    return ESCRIPTTOKEN_FLOAT;
}
void FloatToken::LoadParams(IStream *stream) {
    m_value = stream->ReadFloat();
}
std::string FloatToken::ToString() {
    std::ostringstream ss;
    ss << std::setprecision(2);
    ss << std::fixed;
    ss << m_value << " ";
    return ss.str();
}