#include "VectorToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

VectorToken::VectorToken() {

}
VectorToken::VectorToken(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {

}
VectorToken::~VectorToken() {

}
EScriptToken VectorToken::GetType() {
    return ESCRIPTTOKEN_VECTOR;
}
void VectorToken::LoadParams(IStream *stream) {
    m_x = stream->ReadFloat();
    m_y = stream->ReadFloat();
    m_z = stream->ReadFloat();
}
void VectorToken::Write(IStream *stream) {
    stream->WriteByte(ESCRIPTTOKEN_VECTOR);
    stream->WriteFloat(m_x);
    stream->WriteFloat(m_y);
    stream->WriteFloat(m_z);
}
std::string VectorToken::ToString() {
    std::ostringstream ss;
    ss << "(" << m_x << "," << m_y << "," << m_z << ") ";
    return ss.str();
}