#include "VectorSymbol.h"

#include <sstream>
#include <iomanip>
#include <cassert>

VectorSymbol::VectorSymbol() {

}
VectorSymbol::~VectorSymbol() {

}
ESymbolType VectorSymbol::GetType() {
    return ESYMBOLTYPE_VECTOR;
}
void VectorSymbol::LoadParams(IStream *stream) {
    uint32_t offset = stream->ReadUInt32();
    stream->SetCursor(offset);
    uint32_t hdr = stream->ReadUInt32();
    assert(hdr == 65536);
    
    m_x = stream->ReadFloat();
    m_y = stream->ReadFloat();
    m_z = stream->ReadFloat();
    
}
void VectorSymbol::LoadParamsFromArray(IStream *stream) {
    m_x = stream->ReadFloat();
    m_y = stream->ReadFloat();
    m_z = stream->ReadFloat();
}
std::string VectorSymbol::ToString() {
    std::ostringstream ss;
    ss << "(" << m_x << "," << m_y << "," << m_z << ")";
    return ss.str();
}