#include "IntegerSymbol.h"

#include <sstream>
#include <iomanip>

IntegerSymbol::IntegerSymbol() {

}
IntegerSymbol::~IntegerSymbol() {

}
ESymbolType IntegerSymbol::GetType() {
    return ESYMBOLTYPE_INTEGER;
}
void IntegerSymbol::LoadParams(IStream *stream) {
    m_value = stream->ReadInt32();
}
std::string IntegerSymbol::ToString() {
    std::ostringstream ss;
    ss << m_value << " ";
    return ss.str();
}