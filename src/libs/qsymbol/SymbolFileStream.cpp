#include "SymbolFileStream.h"
#include <cassert>

SymbolFileStream::SymbolFileStream(IStream *data_stream) {
    mp_stream = data_stream;
}
SymbolFileStream::~SymbolFileStream() {

}
QSymbol *SymbolFileStream::NextSymbol() {
    mp_stream->ReadByte();
    uint8_t flags = mp_stream->ReadByte();
    //fprintf(stderr, "flags: %04x - %d\n", flags, mp_stream->GetOffset());

    assert(flags & SYMBOL_ROOT_FLAG);

    uint8_t type = mp_stream->ReadByte();
    //fprintf(stderr, "type: %d\n", type);
    mp_stream->ReadByte();

    uint32_t name_checksum = mp_stream->ReadUInt32();
    //fprintf(stderr, "name_checksum: %08x\n", name_checksum);

    uint32_t source_checksum = mp_stream->ReadUInt32();
    //fprintf(stderr, "source_checksum: %08x\n", source_checksum);

    QSymbol *token = QSymbol::Resolve(type);
    token->SetNameChecksum(name_checksum);
    token->SetSourceChecksum(source_checksum);
    token->LoadParams(mp_stream);

    return token;
}

void SymbolFileStream::WriteHeader() {
    #if WOR_SYMBOLDUMP_HEADER
    mp_stream->WriteUInt32(0);
    mp_stream->WriteUInt32(0);

    mp_stream->WriteUInt32(469762048);
    mp_stream->WriteUInt32(0);
    mp_stream->WriteUInt32(0xFFFFFFFF);
    mp_stream->WriteUInt32(0);
    mp_stream->WriteUInt32(0);
    #else

    mp_stream->WriteUInt32(0);
    mp_stream->WriteUInt32(0);

    mp_stream->WriteUInt32(470286852);
    mp_stream->WriteUInt32(268699660);
    mp_stream->WriteUInt32(201851396);
    mp_stream->WriteUInt32(335676428);
    mp_stream->WriteUInt32(269487104);
    #endif
}
void SymbolFileStream::UpdateHeaderSize() {
    uint32_t offset = mp_stream->GetOffset();
    mp_stream->SetCursor(sizeof(uint32_t));
    mp_stream->WriteUInt32(offset);

    #if WOR_SYMBOLDUMP_HEADER
    mp_stream->SetCursor(sizeof(uint32_t)*4);
    mp_stream->WriteUInt32(offset - 0x1C); //0x1C is the headers length
    #endif
}
void SymbolFileStream::WriteSymbol(QSymbol *symbol) {
    mp_stream->WriteByte(0);
    mp_stream->WriteByte(SYMBOL_ROOT_FLAG);

    mp_stream->WriteByte(symbol->GetType());
    mp_stream->WriteByte(0);
    mp_stream->WriteUInt32(symbol->GetNameChecksum());
    mp_stream->WriteUInt32(m_source_checksum);

    symbol->Write(mp_stream);
}