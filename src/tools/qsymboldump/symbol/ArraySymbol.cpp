#include "ArraySymbol.h"

#include <sstream>
#include <iomanip>

ArraySymbol::ArraySymbol() {

}
ArraySymbol::~ArraySymbol() {

}
ESymbolType ArraySymbol::GetType() {
    return ESYMBOLTYPE_ARRAY;
}
QSymbolToken *NextSymbol(IStream *stream, uint8_t type) {
    QSymbolToken *token = QSymbolToken::Resolve(type);
    token->LoadParams(stream);
    return token;
}
void ArraySymbol::LoadParams(IStream *stream) {
    size_t start_offset = stream->ReadUInt32();
    uint32_t x1 = stream->ReadUInt32();
    uint16_t flags = stream->ReadUInt16();
    uint8_t type = stream->ReadByte();
    stream->ReadByte();
    printf("flags: %p, type: %p, x1: %p\n", flags, type, x1);

    uint32_t num_items = stream->ReadUInt32();
    printf("num items: %d\n", num_items);

    uint32_t offset = stream->ReadUInt32();
    printf("array offset is: %p %p\n", offset, stream->GetOffset());

    uint32_t *offsets = new uint32_t[num_items];
    for(int i=0;i<num_items;i++) {
        //QSymbolToken *token= NextSymbol(stream, type);
        //std::string t = token->ToString();
        //printf("v: %s - %04x\n", t.c_str(), stream->GetOffset());
        offsets[i] = stream->ReadUInt32();
        printf("offset: %d / %p\n", offsets[i], offsets[i]);
    }

    uint32_t cur = stream->GetOffset();
    for(int i=0;i<num_items;i++) {
        stream->SetCursor(offsets[i]);
        printf("Read symbol %d at %p\n", type, offsets[i]);
        QSymbolToken *token = NextSymbol(stream, type);
    }
    stream->SetCursor(cur);

    delete[] offsets;
}
std::string ArraySymbol::ToString() {
    return "";
}