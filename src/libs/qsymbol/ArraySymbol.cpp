#include "ArraySymbol.h"

#include <sstream>
#include <iomanip>
#include <cassert>

ArraySymbol::ArraySymbol() {

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
    printf("Load array params: %08x\n", stream->GetOffset());
    uint32_t array_data_offset = stream->ReadUInt32();
    printf("array data offset: %08x\n", array_data_offset);
    if(m_struct_item) {
        m_next_offset = stream->ReadUInt32();
        printf("next offset: %08x\n", m_next_offset);
    }
    stream->SetCursor(array_data_offset);

    uint16_t hdr = stream->ReadUInt16();
    assert(hdr == 1);

    uint8_t type = stream->ReadByte();
    stream->ReadByte();

    uint32_t num_items = stream->ReadUInt32();

    if(num_items > 1) {
        uint32_t item_data_offset = stream->ReadUInt32();
        stream->SetCursor(item_data_offset);

        uint32_t *offsets = new uint32_t[num_items];
        for(int i=0;i<num_items;i++) {
            offsets[i] = stream->ReadUInt32();
        }

        for(int i=0;i<num_items;i++) {
            stream->SetCursor(offsets[i]);
            QSymbolToken *token = NextSymbol(stream, type);
            m_items.push_back(token);
        }
        delete[] offsets;
    } else {
            QSymbolToken *token = NextSymbol(stream, type);
            m_items.push_back(token);
    }
}

void ArraySymbol::LoadParamsFromArray(IStream *stream) {
    printf("IMPLEMENT ArraySymbol::LoadParamsFromArray\n");
    assert(false);
}
std::string ArraySymbol::ToString() {
    return "";
}