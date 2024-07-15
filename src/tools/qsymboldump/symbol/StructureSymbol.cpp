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
    printf("struct flags: %04x - %04x\n", flags, stream->GetOffset());

    /*if(~flags & 0x01 ) {
        stream->Seek(-sizeof(uint16_t));
        //stream->Seek(-sizeof(uint32_t)); ///XXX: hack
        //stream->Seek(-sizeof(uint32_t)); ///XXX: hack
        printf("** exit StructureSymbol loop: %04x\n", stream->GetOffset());
        return nullptr;
    }*/

    uint8_t type = stream->ReadByte();
    printf("type: %d\n", type);
    stream->ReadByte();

    //uint32_t name_checksum = stream->ReadUInt32();
    //printf("name_checksum: %p\n", name_checksum);

    QSymbolToken *token = QSymbolToken::Resolve(type);
    //token->SetNameChecksum(name_checksum);
    token->LoadParams(stream);
    //stream->ReadInt32(); //??
    return token;
}
void StructureSymbol::LoadParams(IStream *stream) {
    
    printf("StructureSymbol::LoadParams %d / %p\n", stream->GetOffset(), stream->GetOffset());
   

    while(true) {
        stream->ReadInt16();
        uint8_t hdr = stream->ReadByte();
        assert(hdr == 1);

        uint32_t offset = stream->ReadUInt32();
        printf("Seek: %d\n", offset);
        stream->Seek(offset);
        break;
    }
}
std::string StructureSymbol::ToString() {
    std::ostringstream ss;
    
    return ss.str();
}