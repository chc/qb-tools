#include "NameSymbol.h"

#include <sstream>
#include <iomanip>
#include <cassert>
NameSymbol::NameSymbol() {

}
NameSymbol::~NameSymbol() {

}
ESymbolType NameSymbol::GetType() {
    return ESYMBOLTYPE_NAME;
}
void NameSymbol::LoadParams(IStream *stream) {
    m_value = stream->ReadUInt32();
    printf("Name Symbol: %p\n", m_value);
    assert(false);
}
std::string NameSymbol::ToString() {
    std::ostringstream ss;
    ss << m_value << " ";
    return ss.str();
}