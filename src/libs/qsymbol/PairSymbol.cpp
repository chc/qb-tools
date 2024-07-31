#include "PairSymbol.h"

#include <sstream>
#include <iomanip>
#include <cassert>

PairSymbol::PairSymbol() {

}
PairSymbol::~PairSymbol() {

}
PairSymbol::PairSymbol(float x, float y) : m_x(x), m_y(y) {

}
ESymbolType PairSymbol::GetType() {
    return ESYMBOLTYPE_PAIR;
}
void PairSymbol::LoadParams(IStream *stream) {
    uint32_t offset = stream->ReadUInt32();
    if(m_struct_item) {
        m_next_offset = stream->ReadUInt32();
    }
    stream->SetCursor(offset);

    uint32_t hdr = stream->ReadUInt32();
    assert(hdr == 65536);
    
    m_x = stream->ReadFloat();
    m_y = stream->ReadFloat();
}
void PairSymbol::LoadParamsFromArray(IStream *stream) {
    m_x = stream->ReadFloat();
    m_y = stream->ReadFloat();
}
std::string PairSymbol::ToString() {
    std::ostringstream ss;
    ss << "(" << m_x << "," << m_y << ")";
    return ss.str();
}