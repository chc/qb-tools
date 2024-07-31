#include "PairToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

PairToken::PairToken() {

}
PairToken::PairToken(float x, float y) : m_x(x), m_y(y) {

}
PairToken::~PairToken() {

}
EScriptToken PairToken::GetType() {
    return ESCRIPTTOKEN_PAIR;
}
void PairToken::LoadParams(IStream *stream) {
    m_x = stream->ReadFloat();
    m_y = stream->ReadFloat();
}
void PairToken::Write(IStream *stream) {
    stream->WriteByte(ESCRIPTTOKEN_PAIR);
    stream->WriteFloat(m_x);
    stream->WriteFloat(m_y);
}
std::string PairToken::ToString() {
    std::ostringstream ss;
    ss << "(" << m_x << "," << m_y << ") ";
    return ss.str();
}