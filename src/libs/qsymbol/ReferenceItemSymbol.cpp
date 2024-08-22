#include "ReferenceItemSymbol.h"

#include <sstream>
#include <iomanip>
#include <cassert>
ReferenceItemSymbol::ReferenceItemSymbol(uint8_t type, bool required) : m_type(type) , m_is_required_param(required) {
    #ifndef SYMBOL_STRUCT_REQUIRED_PARAM_FLAG
    assert(type == ESYMBOLTYPE_STRUCTURE);
    #endif
}
ReferenceItemSymbol::~ReferenceItemSymbol() {

}
ESymbolType ReferenceItemSymbol::GetType() {
    return ESYMBOLTYPE_INTERNAL_REFERENCE;
}
void ReferenceItemSymbol::LoadParams(IStream *stream) {
    SetValue(stream->ReadUInt32());
    SetNextOffset(stream->ReadUInt32());
}
void ReferenceItemSymbol::LoadParamsFromArray(IStream *stream) {
    SetValue(stream->ReadUInt32());
}
void ReferenceItemSymbol::Write(IStream *stream) {
    stream->WriteUInt32(m_value);
    stream->WriteUInt32(0xFFFFFFFF);
}
void ReferenceItemSymbol::WriteToArray(IStream *stream) {
    stream->WriteUInt32(m_value);
    if(m_struct_item) {
        //set next_offset
        uint32_t cursor = stream->GetOffset();
        if(m_next_offset == 1) {
            stream->WriteUInt32(cursor + sizeof(int32_t));   
        } else {
            stream->WriteUInt32(0);
        }
    }
}
std::string ReferenceItemSymbol::ToString() {
    return "";
}