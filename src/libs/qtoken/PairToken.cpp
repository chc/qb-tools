#include "PairToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

PairToken::PairToken() {

}
PairToken::~PairToken() {

}
EScriptToken PairToken::GetType() {
    return ESCRIPTTOKEN_PAIR;
}
void PairToken::LoadParams(IStream *stream) {
    m_values[0] = stream->ReadFloat();
    m_values[1] = stream->ReadFloat();
}
std::string PairToken::ToString() {
    std::ostringstream ss;
    ss << "(" << m_values[0] << "," << m_values[1] << ") ";
    return ss.str();
}