#include "LocalStringSymbol.h"

#include <iostream>
#include <sstream>
#include <iomanip>

LocalStringSymbol::LocalStringSymbol() {

}
LocalStringSymbol::~LocalStringSymbol() {

}
ESymbolType LocalStringSymbol::GetType() {
    return ESYMBOLTYPE_LOCALSTRING;
}
void LocalStringSymbol::LoadParams(IStream *stream) {
    uint32_t offset = stream->ReadUInt32();
    if(m_struct_item) {
        m_next_offset = stream->ReadUInt32();
    }
    stream->SetCursor(offset);

    const int MAX_CHARS = 256;
    char16_t data[MAX_CHARS];
    uint32_t i = 0;

    while(true) {
        data[i % MAX_CHARS] = (char16_t)stream->ReadUInt16();
        if(data[i % MAX_CHARS] == 0) {
            break;
        }
        m_value += (uint8_t)data[i];
        i++;
    }
        
    stream->Align();
}
void LocalStringSymbol::LoadParamsFromArray(IStream *stream) {
    const int MAX_CHARS = 256;
    char16_t data[MAX_CHARS];
    uint32_t i = 0;

    while(true) {
        data[i % MAX_CHARS] = (char16_t)stream->ReadUInt16();
        if(data[i % MAX_CHARS] == 0) {
            break;
        }
        m_value += (uint8_t)data[i];
        i++;
    }
        
    stream->Align();
}
std::string LocalStringSymbol::ToString() {
    return "";
}