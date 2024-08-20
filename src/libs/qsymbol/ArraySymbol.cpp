#include "ArraySymbol.h"
#include "ReferenceItemSymbol.h"
#include <sstream>
#include <iomanip>
#include <cassert>

ArraySymbol::ArraySymbol() {
    m_struct_item = false;

}
ArraySymbol::ArraySymbol(std::vector<QSymbolToken *> tokens) {
    m_struct_item = false;
    m_num_items = tokens.size();
    m_tokens = new QSymbolToken*[m_num_items];
    for(int i=0;i<m_num_items;i++) {
        m_tokens[i] = tokens.at(i);
        m_tokens[i]->SetIsStructItem(false);
    }
}
ArraySymbol::ArraySymbol(QSymbolToken **tokens, uint32_t num_tokens) : m_tokens(tokens), m_num_items(num_tokens) {
    m_struct_item = false;
}
ArraySymbol::~ArraySymbol() {

}
ESymbolType ArraySymbol::GetType() {
    return ESYMBOLTYPE_ARRAY;
}
QSymbolToken *NextSymbol(IStream *stream, uint8_t type) {
    QSymbolToken *token = QSymbolToken::Resolve(type);
    token->LoadParamsFromArray(stream);
    return token;
}
void ArraySymbol::LoadParams(IStream *stream) {

    uint32_t array_data_offset = stream->ReadUInt32();
    
    if(m_struct_item) {
        m_next_offset = stream->ReadUInt32();
    }

    stream->SetCursor(array_data_offset);

    LoadParamsFromArray(stream);
}

void ArraySymbol::LoadParamsFromArray(IStream *stream) {
    stream->ReadByte();
    uint8_t hdr = stream->ReadByte();
    assert(hdr == 1);

    uint8_t type = stream->ReadByte();
    stream->ReadByte();

    m_num_items = stream->ReadUInt32();

    m_tokens = new QSymbolToken*[m_num_items];

    bool is_ref = false;
    if(type & SYMBOL_ISREF_FLAG) {
        is_ref = true;
        type &= ~SYMBOL_ISREF_FLAG;
    }

    ReadSymbolsFromArray(stream, type, is_ref, m_num_items, m_tokens);
}
void ArraySymbol::Write(IStream *stream) {

    uint32_t offset = stream->GetOffset() + sizeof(uint32_t) + sizeof(uint32_t);
    stream->WriteUInt32(offset); //offset
    stream->WriteUInt32(0);
    WriteNoOffset(stream);
}
void ArraySymbol::WriteNoOffset(IStream *stream) {
    stream->WriteByte(0);
    stream->WriteByte(1);

    uint8_t type_flags = 0;
    uint8_t type = 0;
    bool is_ref = false;
    if(m_num_items > 0) {
        if(m_tokens[0]->GetType() == ESYMBOLTYPE_INTERNAL_REFERENCE) {
            type = reinterpret_cast<ReferenceItemSymbol*>(m_tokens[0])->GetRefType();
            type_flags = type;
            type_flags |= SYMBOL_ISREF_FLAG;
            is_ref = true;
        } else {
            type = m_tokens[0]->GetType();
            type_flags = type;
        }
    }
    stream->WriteByte(type_flags);
    stream->WriteByte(0);

    stream->WriteUInt32(m_num_items);
    stream->WriteAlign();

    WriteSymbolsToArray(stream, type, is_ref, m_num_items, m_tokens);
}
void ArraySymbol::WriteToArray(IStream *stream) {
    uint32_t start_offset = stream->GetOffset();
    if(m_struct_item) {
        stream->WriteUInt32(0); //next_offset
    }

    WriteNoOffset(stream);

    if(m_struct_item) {
        //set next_offset
        uint32_t cursor = stream->GetOffset();
        stream->SetCursor(start_offset);
        if(m_next_offset) {
            stream->WriteUInt32(cursor);
        } else {
            stream->WriteUInt32(0);
        }
        
        stream->SetCursor(cursor);
    }
}
std::string ArraySymbol::ToString() {
    return "";
}