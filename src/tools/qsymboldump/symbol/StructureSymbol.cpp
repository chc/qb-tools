#include "StructureSymbol.h"

#include <sstream>
#include <iomanip>
#include <cassert>
#include "../SStream.h"

StructureSymbol::StructureSymbol() {

}
StructureSymbol::~StructureSymbol() {

}
ESymbolType StructureSymbol::GetType() {
    return ESYMBOLTYPE_STRUCTURE;
}
QSymbolToken *StructureSymbol::NextSymbol(IStream *stream) {
    uint16_t flags = stream->ReadUInt16();
    uint8_t type = stream->ReadByte();
    stream->ReadByte();
    
    QSymbolToken *token = QSymbolToken::Resolve(type);
    token->LoadParams(stream);
    return token;
}
void StructureSymbol::LoadParamsFromArray(IStream *stream) {
   
    printf("StructureSymbol::LoadParamsFromArray %d / %p\n", stream->GetOffset(), stream->GetOffset());

    stream->ReadInt16();
    uint8_t hdr = stream->ReadByte();
    assert(hdr == 1);
    stream->ReadByte();

    uint32_t offset = stream->ReadUInt32();
    assert(offset == stream->GetOffset());
    while(true) {
        uint8_t unk = stream->ReadByte();
        uint8_t type_flags = stream->ReadByte();
        assert(type_flags & 0x80);
        uint8_t type = type_flags & 0xF;

        uint16_t unk2 = stream->ReadUInt16();
        assert(unk2 == 0);

        uint32_t name = stream->ReadUInt32();

        QSymbolToken *token = QSymbolToken::Resolve(type);
        token->SetNameChecksum(name);     
        token->LoadParams(stream);

        uint32_t next = token->GetNextOffset();
        if(next == 0) {
            break;
        }
    }
}
void StructureSymbol::LoadParams(IStream *stream) {
    
    stream->ReadInt16();
    uint8_t hdr = stream->ReadByte();
    assert(hdr == 1);
    stream->ReadByte();

    uint32_t offset = stream->ReadUInt32();
    assert(offset = stream->GetOffset());
    while(true) {
        uint8_t unk = stream->ReadByte();
        uint8_t type_flags = stream->ReadByte();
        assert(type_flags & 0x80);
        uint8_t type = type_flags & 0xF;

        uint16_t unk2 = stream->ReadUInt16();
        assert(unk2 == 0);

        uint32_t name = stream->ReadUInt32();

        QSymbolToken *token = QSymbolToken::Resolve(type);
        token->SetNameChecksum(name);
        
        token->LoadParams(stream);

        uint32_t next = token->GetNextOffset();
        if(next == 0) {
            break;
        }
    }
}
std::string StructureSymbol::ToString() {
    std::ostringstream ss;
    
    return ss.str();
}