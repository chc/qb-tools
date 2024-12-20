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
    
    uint8_t hdr = stream->ReadByte();
    assert(hdr == 0);
    hdr = stream->ReadByte();
    assert(hdr == 1);
    hdr = stream->ReadByte();
    assert(hdr == 0);
    hdr = stream->ReadByte();
    assert(hdr == 0);
    
    m_x = stream->ReadFloat();
    m_y = stream->ReadFloat();
    m_z = stream->ReadFloat();
    
}
void VectorSymbol::LoadParamsFromArray(IStream *stream) {
    uint8_t hdr = stream->ReadByte();
    assert(hdr == 0);
    hdr = stream->ReadByte();
    assert(hdr == 1);
    hdr = stream->ReadByte();
    assert(hdr == 0);
    hdr = stream->ReadByte();
    assert(hdr == 0);
    
    
    m_x = stream->ReadFloat();
    m_y = stream->ReadFloat();
    m_z = stream->ReadFloat();
}

void VectorSymbol::Write(IStream *stream) {
    uint32_t offset = stream->GetOffset() + sizeof(uint32_t);
    
    if(m_struct_item) {
        offset += sizeof(uint32_t);
    }
    stream->WriteUInt32(offset);

    uint32_t next_cursor = stream->GetOffset();
    if(m_struct_item) {
        stream->WriteUInt32(0);
    }

    stream->WriteByte(0);
    stream->WriteByte(1);
    stream->WriteByte(0);
    stream->WriteByte(0);
    stream->WriteFloat(m_x);
    stream->WriteFloat(m_y);
    stream->WriteFloat(m_z);

    if(m_struct_item && m_next_offset) {
        uint32_t cursor = stream->GetOffset();
        stream->SetCursor(next_cursor);
        stream->WriteUInt32(cursor);
        stream->SetCursor(cursor);

    }
}
void VectorSymbol::WriteToArray(IStream *stream) {
    uint32_t next_cursor = stream->GetOffset();
    if(m_struct_item) {
        stream->WriteUInt32(0);
    }
    stream->WriteByte(0);
    stream->WriteByte(1);
    stream->WriteByte(0);
    stream->WriteByte(0);
    stream->WriteFloat(m_x);
    stream->WriteFloat(m_y);
    stream->WriteFloat(m_z);

    if(m_struct_item && m_next_offset) {
        uint32_t cursor = stream->GetOffset();
        stream->SetCursor(next_cursor);
        stream->WriteUInt32(cursor);
        stream->SetCursor(cursor);

    }
}
std::string VectorSymbol::ToString() {
    std::ostringstream ss;
    ss << "Vec(" << m_x << "," << m_y << "," << m_z << ")";
    return ss.str();
}