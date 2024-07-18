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
    size_t start_offset = stream->ReadUInt32();
    assert(start_offset == stream->GetOffset()+4);
    //uint32_t x1 = stream->ReadUInt32();
    //assert(x1 == 0); //end_offset?
    m_next_offset = stream->ReadUInt32();
    uint16_t flags = stream->ReadUInt16();
    uint8_t type = stream->ReadByte();
    stream->ReadByte();

    uint32_t num_items = stream->ReadUInt32();

    uint32_t weird_item = stream->ReadUInt32();
    if(type == ESYMBOLTYPE_STRUCTURE) {
        uint32_t first_item = stream->ReadUInt32();
        uint32_t next_item = stream->ReadUInt32();
        printf("first_item: %d %d - %d\n", first_item, next_item, stream->GetOffset());
    } else {
        assert(weird_item == stream->GetOffset());
        printf("weird_item: %d %d\n", weird_item, stream->GetOffset());
    }

    
    for(int i=0;i<num_items;i++) {
        QSymbolToken *token = NextSymbol(stream, type);
    }
}

void ArraySymbol::LoadParamsFromArray(IStream *stream) {
    printf("IMPLEMENT ArraySymbol::LoadParamsFromArray\n");
    assert(false);
}
std::string ArraySymbol::ToString() {
    return "";
}