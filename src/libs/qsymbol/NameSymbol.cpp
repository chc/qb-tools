#include "NameSymbol.h"

#include <sstream>
#include <iomanip>
#include <cassert>
NameSymbol::NameSymbol() {

}
NameSymbol::NameSymbol(uint32_t v) : m_value(v){

}
NameSymbol::~NameSymbol() {

}
ESymbolType NameSymbol::GetType() {
    return ESYMBOLTYPE_NAME;
}
void NameSymbol::LoadParams(IStream *stream) {
    m_value = stream->ReadUInt32();
    if(m_struct_item) {
        m_next_offset = stream->ReadUInt32();
    } else {
        stream->ReadUInt32();
    }
}
void NameSymbol::LoadParamsFromArray(IStream *stream) {
    m_value = stream->ReadUInt32();
}
void NameSymbol::Write(IStream *stream) {
    stream->WriteInt32(m_value);
    stream->WriteUInt32(0);
}
void NameSymbol::WriteToArray(IStream *stream) {
    stream->WriteInt32(m_value);
    if(m_struct_item) {
        //set next_offset
        uint32_t cursor = stream->GetOffset();
        if(m_next_offset) {
            stream->WriteUInt32(cursor + sizeof(uint32_t));   
        } else {
            stream->WriteUInt32(0);
        }
    }
}
std::string NameSymbol::ToString() {
    std::ostringstream ss;
    ss << m_value << " ";
    return ss.str();
}