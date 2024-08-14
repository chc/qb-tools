#include "SymbolFileStream.h"
#include <cassert>

SymbolFileStream::SymbolFileStream(IStream *data_stream) {
    mp_stream = data_stream;
}
SymbolFileStream::~SymbolFileStream() {

}
QSymbolToken *SymbolFileStream::NextSymbol() {
    uint16_t flags = mp_stream->ReadUInt16();
    //printf("flags: %04x - %d\n", flags, mp_stream->GetOffset());

    assert(flags & 0x20);

    uint8_t type = mp_stream->ReadByte();
    //printf("type: %d\n", type);
    mp_stream->ReadByte();

    uint32_t name_checksum = mp_stream->ReadUInt32();
    //printf("name_checksum: %08x\n", name_checksum);

    uint32_t source_checksum = mp_stream->ReadUInt32();
    //printf("source_checksum: %08x\n", source_checksum);

    QSymbolToken *token = QSymbolToken::Resolve(type);
    token->SetNameChecksum(name_checksum);
    token->SetSourceChecksum(source_checksum);
    token->LoadParams(mp_stream);

    return token;
}

void SymbolFileStream::WriteHeader() {
    mp_stream->WriteUInt32(0);
    mp_stream->WriteUInt32(0);

    mp_stream->WriteUInt32(470286852);
    mp_stream->WriteUInt32(268699660);
    mp_stream->WriteUInt32(201851396);
    mp_stream->WriteUInt32(335676428);
    mp_stream->WriteUInt32(269487104);
}
void SymbolFileStream::UpdateHeaderSize() {
    uint32_t offset = mp_stream->GetOffset();
    mp_stream->SetCursor(sizeof(uint32_t));
    mp_stream->WriteUInt32(offset);
}
void SymbolFileStream::WriteSymbol(QSymbolToken *symbol) {
    mp_stream->WriteUInt16(0x20);

    mp_stream->WriteByte(symbol->GetType());
    mp_stream->WriteByte(0);
    mp_stream->WriteUInt32(symbol->GetNameChecksum());
    mp_stream->WriteUInt32(m_source_checksum);

    symbol->Write(mp_stream);
}