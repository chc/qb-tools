#include "StringSymbol.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cassert>
StringSymbol::StringSymbol() {

}
StringSymbol::StringSymbol(std::string value) : m_value(value) {
}
StringSymbol::~StringSymbol() {

}
ESymbolType StringSymbol::GetType() {
    return ESYMBOLTYPE_STRING;
}
void StringSymbol::LoadParams(IStream *stream) {
    uint32_t offset = stream->ReadUInt32();    
    if(m_struct_item) {
        m_next_offset = stream->ReadUInt32();
    }
    stream->SetCursor(offset);

    const int MAX_CHARS = 256;
    char data[MAX_CHARS];
    uint32_t i = 0;

    while(true) {
        data[i % MAX_CHARS] = (char)stream->ReadByte();
        if(data[i % MAX_CHARS] == 0) {
            break;
        }
        i++;
    }

    m_value = (char *)&data[0];
    
    stream->Align();
}
void StringSymbol::LoadParamsFromArray(IStream *stream) {
    const int MAX_CHARS = 256;
    char data[MAX_CHARS];
    uint32_t i = 0;

    while(true) {
        data[i % MAX_CHARS] = (char)stream->ReadByte();
        if(data[i % MAX_CHARS] == 0) {
            break;
        }
        i++;
    }

    m_value = (char *)&data[0];
    stream->Align();
}
void StringSymbol::Write(IStream *stream) {
    WriteToArray(stream);
}
void StringSymbol::WriteToArray(IStream *stream) {
    uint32_t start = stream->GetOffset();
    stream->WriteUInt32(0);

    stream->WriteBuffer((uint8_t *)m_value.c_str(), m_value.length() + 1);
    stream->WriteAlign();

    if(m_struct_item && m_next_offset == 1) {
        //set next_offset
        uint32_t cursor = stream->GetOffset();
        stream->SetCursor(start);
        stream->WriteUInt32(cursor);   
        stream->SetCursor(cursor);
        
    }
}
std::string StringSymbol::ToString() {
    return "";
}