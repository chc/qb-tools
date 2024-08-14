#include "ArraySymbol.h"

#include <sstream>
#include <iomanip>
#include <cassert>

ArraySymbol::ArraySymbol() {
    m_struct_item = false;

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

    uint16_t hdr = stream->ReadUInt16();
    assert(hdr == 1);

    uint8_t type_flags = stream->ReadByte();
    stream->ReadByte();

    m_num_items = stream->ReadUInt32();

    m_tokens = new QSymbolToken*[m_num_items];

    ReadSymbolsFromArray(stream, type_flags, m_num_items, m_tokens);
}

void ArraySymbol::LoadParamsFromArray(IStream *stream) {
    uint16_t hdr = stream->ReadUInt16();
    assert(hdr == 1);

    uint8_t type_flags = stream->ReadByte();
    stream->ReadByte();

    m_num_items = stream->ReadUInt32();

    m_tokens = new QSymbolToken*[m_num_items];

    ReadSymbolsFromArray(stream, type_flags, m_num_items, m_tokens);
}
void ArraySymbol::Write(IStream *stream) {
    uint32_t offset = stream->GetOffset() + sizeof(uint32_t) + sizeof(uint32_t);
    stream->WriteUInt32(offset); //offset
    stream->WriteUInt32(0);
    stream->WriteUInt16(1);

    uint8_t type = 0;
    type = m_tokens[0]->GetType();
    stream->WriteByte(type);
    stream->WriteByte(0);

    stream->WriteUInt32(m_num_items);

    WriteSymbolsToArray(stream, type, m_num_items, m_tokens);
}
void ArraySymbol::WriteToArray(IStream *stream) {
    assert(false);
}
std::string ArraySymbol::ToString() {
    return "";
}