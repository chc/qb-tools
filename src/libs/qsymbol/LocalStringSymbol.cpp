#include "LocalStringSymbol.h"

#include <iostream>
#include <sstream>
#include <iomanip>

LocalStringSymbol::LocalStringSymbol() {

}
LocalStringSymbol::LocalStringSymbol(std::string value) : m_value(value) {

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

void LocalStringSymbol::Write(IStream *stream) {
    uint32_t start = stream->GetOffset();
    stream->WriteUInt32(start + sizeof(uint32_t) + sizeof(uint32_t));
    stream->WriteUInt32(0);

    std::string::iterator it = m_value.begin();
    while(it != m_value.end()) {
        char16_t c = *it;
        stream->WriteUInt16(c);
        it++;
    }
    stream->WriteUInt16(0);
    
    stream->WriteAlign();

    if(m_struct_item && m_next_offset == 1) {
        //set next_offset
        uint32_t cursor = stream->GetOffset();
        stream->SetCursor(start + sizeof(uint32_t));
        stream->WriteUInt32(cursor);   
        stream->SetCursor(cursor);
        
    }
}
void LocalStringSymbol::WriteToArray(IStream *stream) {
    Write(stream);
}
std::string LocalStringSymbol::ToString() {
    return "";
}