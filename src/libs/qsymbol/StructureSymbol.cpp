#include "StructureSymbol.h"
#include "ReferenceItemSymbol.h"


#include <sstream>
#include <iomanip>
#include <cassert>

StructureSymbol::StructureSymbol() {

}
StructureSymbol::StructureSymbol(std::vector<QSymbol *> children) : m_children(children) {
}
StructureSymbol::~StructureSymbol() {

}
ESymbolType StructureSymbol::GetType() {
    return ESYMBOLTYPE_STRUCTURE;
}
QSymbol *StructureSymbol::NextSymbol(IStream *stream) {
    uint16_t flags = stream->ReadUInt16();
    uint8_t type = stream->ReadByte();
    stream->ReadByte();
    
    QSymbol *token = QSymbol::Resolve(type);
    token->LoadParams(stream);
    return token;
}
void StructureSymbol::LoadParamsFromArray(IStream *stream) {
    LoadParamsNoOffset(stream);
}
void StructureSymbol::LoadParams(IStream *stream) {
    uint32_t struct_offset = stream->ReadUInt32();
    m_next_offset = stream->ReadUInt32();

    stream->SetCursor(struct_offset);

    LoadParamsNoOffset(stream);
}
void StructureSymbol::LoadParamsNoOffset(IStream *stream) {
    uint8_t hdr = stream->ReadByte();
    assert(hdr == 0);
    hdr = stream->ReadByte();
    assert(hdr == 0);
    hdr = stream->ReadByte();
    assert(hdr == 1);
    hdr = stream->ReadByte();
    assert(hdr == 0);
    

    uint32_t offset = stream->ReadUInt32();
    assert(offset == 0 || offset == stream->GetOffset());
    
    if(offset == 0) { //if 0, emptry struct
        return;
    }
    while(true) {

        bool is_ref = false;
        bool is_required_params_info = false;
        #ifdef SYMBOL_STRUCT_SEPERATE_FLAGS
        uint8_t unk = stream->ReadByte();
        assert(unk == 0);
        uint8_t flags = stream->ReadByte();
        assert(flags & SYMBOL_STRUCT_FLAG);
        uint8_t type = stream->ReadByte();
        unk = stream->ReadByte();
        assert(unk == 0);
        if(type & SYMBOL_ISREF_FLAG) {
            is_ref = true;
            type &= SYMBOL_STRUCT_TYPE_ANDMASK;
        }
        if(type & SYMBOL_STRUCT_REQUIRED_PARAM_FLAG) {
            is_ref = true;
            type &= ~SYMBOL_STRUCT_REQUIRED_PARAM_FLAG;
            is_required_params_info = true;
        }
        #else
        uint8_t unk = stream->ReadByte();
        assert(unk == 0);
        uint8_t type_flags = stream->ReadByte();
        assert(type_flags & SYMBOL_STRUCT_FLAG);
        uint8_t type = (type_flags & SYMBOL_STRUCT_TYPE_ANDMASK) >> SYMBOL_STRUCT_TYPE_RSHIFTMASK;
        uint16_t unk2 = stream->ReadUInt16();
        assert(unk2 == 0);

        if(type_flags & SYMBOL_ISREF_FLAG) {
            is_ref = true;
        }
        #endif

        uint32_t name = stream->ReadUInt32();

        QSymbol *token = NULL;
        if(is_ref) {
            ReferenceItemSymbol *ref = new ReferenceItemSymbol(type, is_required_params_info);
            ref->SetIsStructItem(true);
            ref->SetValue(stream->ReadUInt32());
            #ifdef SYMBOL_STRUCT_REQUIRED_PARAM_FLAG
            if(is_required_params_info) {
                assert(ref->GetValue() == 0x69696969);
            }
            #endif
            
            ref->SetNextOffset(stream->ReadUInt32());
            token = ref;
        } else {
            token = QSymbol::Resolve(type);
            token->SetIsStructItem(true);
            token->LoadParams(stream);
        }
        token->SetNameChecksum(name);
        m_children.push_back(token);


        uint32_t next = token->GetNextOffset();
        if(next == 0) {
            break;
        }

        stream->SetCursor(next);
    }
}
void StructureSymbol::WriteSymbol(IStream *stream, QSymbol *sym) {
    bool is_ref = false;
    uint8_t type = sym->GetType();
    uint8_t flags = (type << SYMBOL_STRUCT_TYPE_RSHIFTMASK);
    
    if(type  == ESYMBOLTYPE_INTERNAL_REFERENCE) {
        ReferenceItemSymbol* ref = reinterpret_cast<ReferenceItemSymbol*>(sym);
        type = ref->GetRefType();
        flags = (type) << SYMBOL_STRUCT_TYPE_RSHIFTMASK;
        if(ref->GetIsRequiredParams()) {
            assert(ref->GetNameChecksum() != 0);
            ref->SetValue(0x69696969);
            #ifdef SYMBOL_STRUCT_REQUIRED_PARAM_FLAG 
            flags |= SYMBOL_STRUCT_REQUIRED_PARAM_FLAG;
            #else
            assert(false);
            #endif
        } else {
            flags |= SYMBOL_ISREF_FLAG;
        }
        
        is_ref = true;
    }
    

    #ifdef SYMBOL_STRUCT_SEPERATE_FLAGS
    stream->WriteByte(0);
    stream->WriteByte(SYMBOL_STRUCT_FLAG);
    stream->WriteByte(flags);
    stream->WriteByte(0);
    #else
    flags |= SYMBOL_STRUCT_FLAG;
    stream->WriteByte(0);
    stream->WriteByte(flags);
    stream->WriteUInt16(0);
    #endif
    stream->WriteUInt32(sym->GetNameChecksum());

    QSymbol *tokens[1] = {sym};
    sym->SetIsStructItem(true);
    WriteSymbolsToArray(stream, type, is_ref, 1, tokens);
}
void StructureSymbol::Write(IStream *stream) {

    stream->WriteUInt32(stream->GetOffset() + sizeof(uint32_t));
    if(m_struct_item) {
        stream->WriteUInt32(0);
    }
    
    WriteNoOffset(stream);
}

void StructureSymbol::WriteNoOffset(IStream *stream) {
    stream->WriteByte(0);
    stream->WriteByte(0);
    stream->WriteByte(1);
    stream->WriteByte(0);

    if(m_children.empty()) {
        stream->WriteUInt32(0);
        return;
    }
   stream->WriteUInt32(stream->GetOffset() + sizeof(uint32_t));

    std::vector<QSymbol *>::iterator it = m_children.begin();
    while(it != m_children.end()) {
        QSymbol *sym = *it;
        it++;
        bool has_next = it != m_children.end();
        if(has_next) {
            sym->SetNextOffset(1);
        }
        WriteSymbol(stream, sym);
    }
}

void StructureSymbol::WriteToArray(IStream *stream) {
    uint32_t start_cursor = stream->GetOffset();
    if(m_struct_item) {
        stream->WriteUInt32(0);    
    }
    
    WriteNoOffset(stream);

    if(m_struct_item && m_next_offset) {
        uint32_t cursor = stream->GetOffset();
        stream->SetCursor(start_cursor);
        
        stream->WriteUInt32(cursor);    

        stream->SetCursor(cursor);
    }

}
std::string StructureSymbol::ToString() {
    std::ostringstream ss;
    
    return ss.str();
}