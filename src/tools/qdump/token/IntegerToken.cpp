#include "IntegerToken.h"
#include "../IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

IntegerToken::IntegerToken() {

}
IntegerToken::~IntegerToken() {

}
EScriptToken IntegerToken::GetType() {
    return ESCRIPTTOKEN_INTEGER;
}
void IntegerToken::LoadParams(IQStream *stream) {
    m_value = stream->ReadInt32();
}
std::string IntegerToken::ToString() {
    std::ostringstream ss;
    ss << m_value << " ";
    return ss.str();
}