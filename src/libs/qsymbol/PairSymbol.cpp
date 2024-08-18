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
    uint32_t hdr = stream->ReadUInt32();
    assert(hdr == 65536);
    
    m_x = stream->ReadFloat();
    m_y = stream->ReadFloat();
}
void PairSymbol::Write(IStream *stream) {
    uint32_t offset = stream->GetOffset() + sizeof(uint32_t);
    
    if(m_struct_item) {
        offset += sizeof(uint32_t);
    }
    stream->WriteUInt32(offset);

    uint32_t next_cursor = stream->GetOffset();
    if(m_struct_item) {
        stream->WriteUInt32(0);
    }

    stream->WriteUInt32(65536);
    stream->WriteFloat(m_x);
    stream->WriteFloat(m_y);

    if(m_struct_item && m_next_offset) {
        uint32_t cursor = stream->GetOffset();
        stream->SetCursor(next_cursor);
        stream->WriteUInt32(cursor);
        stream->SetCursor(cursor);

    }
}
void PairSymbol::WriteToArray(IStream *stream) {
    stream->WriteUInt32(65536);
    stream->WriteFloat(m_x);
    stream->WriteFloat(m_y);
}
std::string PairSymbol::ToString() {
    std::ostringstream ss;
    ss << "(" << m_x << "," << m_y << ")";
    return ss.str();
}