#include "VectorSymbol.h"

#include <sstream>
#include <iomanip>
#include <cassert>

VectorSymbol::VectorSymbol() {

}
VectorSymbol::VectorSymbol(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {

}
VectorSymbol::~VectorSymbol() {

}
ESymbolType VectorSymbol::GetType() {
    return ESYMBOLTYPE_VECTOR;
}
void VectorSymbol::LoadParams(IStream *stream) {
    uint32_t offset = stream->ReadUInt32();
    if(m_struct_item) {
        m_next_offset = stream->ReadUInt32();
    }
    stream->SetCursor(offset);
    
    uint32_t hdr = stream->ReadUInt32();
    assert(hdr == 65536);
    
    m_x = stream->ReadFloat();
    m_y = stream->ReadFloat();
    m_z = stream->ReadFloat();
    
}
void VectorSymbol::LoadParamsFromArray(IStream *stream) {
    uint32_t hdr = stream->ReadUInt32();
    assert(hdr == 65536);
    
    m_x = stream->ReadFloat();
    m_y = stream->ReadFloat();
    m_z = stream->ReadFloat();
}
#include <cassert>
void VectorSymbol::Write(IStream *stream) {
    assert(0);
}
void VectorSymbol::WriteToArray(IStream *stream) {
    assert(0);
}
std::string VectorSymbol::ToString() {
    std::ostringstream ss;
    ss << "(" << m_x << "," << m_y << "," << m_z << ")";
    return ss.str();
}