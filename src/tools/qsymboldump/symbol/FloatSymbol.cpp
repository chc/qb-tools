#include "FloatSymbol.h"

#include <sstream>
#include <iomanip>

FloatSymbol::FloatSymbol() {

}
FloatSymbol::~FloatSymbol() {

}
ESymbolType FloatSymbol::GetType() {
    return ESYMBOLTYPE_FLOAT;
}
void FloatSymbol::LoadParams(IStream *stream) {
    m_value = stream->ReadFloat();
}
std::string FloatSymbol::ToString() {
    std::ostringstream ss;
    ss << m_value << " ";
    return ss.str();
}