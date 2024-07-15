#include "SStream.h"

SStream::SStream(IStream *data_stream) {
    mp_stream = data_stream;
}
SStream::~SStream() {

}
QSymbolToken *SStream::NextSymbol() {
    uint16_t flags = mp_stream->ReadUInt16();
    //printf("flags: %04x - %d\n", flags, mp_stream->GetOffset());

    if(flags == 0) {
        return nullptr;
    }

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