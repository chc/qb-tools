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
        #ifdef WIDE_LOCALSTRING
        data[i % MAX_CHARS] = (char16_t)stream->ReadUInt16();
        #else
        data[i % MAX_CHARS] = (char16_t)stream->ReadByte();
        #endif
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
        #ifdef WIDE_LOCALSTRING
        data[i % MAX_CHARS] = (char16_t)stream->ReadUInt16();
        #else
        data[i % MAX_CHARS] = (char16_t)stream->ReadByte();
        #endif
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
    writeData(stream);

    if(m_struct_item && m_next_offset == 1) {
        //set next_offset
        uint32_t cursor = stream->GetOffset();
        stream->SetCursor(start);
        stream->WriteUInt32(cursor);   
        stream->SetCursor(cursor);
        
    }

}
void LocalStringSymbol::writeData(IStream *stream) {
    std::string::iterator it = m_value.begin();
    while(it != m_value.end()) {
        char16_t c = *it;
        #ifdef WIDE_LOCALSTRING
        stream->WriteUInt16(c);
        #else
        stream->WriteByte(c);
        #endif
        it++;
    }
    #ifdef WIDE_LOCALSTRING
    stream->WriteUInt16(0);
    #else
    stream->WriteByte(0);
    #endif
    
    stream->WriteAlign();


}
void LocalStringSymbol::WriteToArray(IStream *stream) {
    uint32_t start = stream->GetOffset();
    if(m_struct_item) {
        stream->WriteUInt32(0);
    }
    writeData(stream);
    if(m_struct_item && m_next_offset == 1) {
        //set next_offset
        uint32_t cursor = stream->GetOffset();
        stream->SetCursor(start);
        stream->WriteUInt32(cursor);   
        stream->SetCursor(cursor);
        
    }
}
std::string LocalStringSymbol::ToString() {
    return "";
}