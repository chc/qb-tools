#include "VectorToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

VectorToken::VectorToken() {

}
VectorToken::~VectorToken() {

}
EScriptToken VectorToken::GetType() {
    return ESCRIPTTOKEN_PAIR;
}
void VectorToken::LoadParams(IStream *stream) {
    m_values[0] = stream->ReadFloat();
    m_values[1] = stream->ReadFloat();
    m_values[3] = stream->ReadFloat();
}
std::string VectorToken::ToString() {
    std::ostringstream ss;
    ss << "(" << m_values[0] << "," << m_values[1] << "," << m_values[2] << ") ";
    return ss.str();
}