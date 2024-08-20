#include "QSymbolToken.h"
#include <cassert>

#include "IntegerSymbol.h"
#include "NameSymbol.h"
#include "LocalStringSymbol.h"
#include "StringSymbol.h"
#include "FloatSymbol.h"
#include "PairSymbol.h"
#include "VectorSymbol.h"
#include "ArraySymbol.h"
#include "StructureSymbol.h"
#include "QScriptSymbol.h"
#include "ReferenceItemSymbol.h"
QSymbol::QSymbol() {
    m_next_offset = 0;
    m_name_checksum = 0;
    m_source_checksum = 0;
    m_struct_item = false;

}
QSymbol::~QSymbol() {

}

QSymbol* QSymbol::Resolve(uint8_t token) {
    switch(token) {
        case ESYMBOLTYPE_INTEGER:
            return new IntegerSymbol();
        case ESYMBOLTYPE_NAME:
            return new NameSymbol();
        case ESYMBOLTYPE_STRING:
            return new StringSymbol();
        case ESYMBOLTYPE_LOCALSTRING:
            return new LocalStringSymbol();
        case ESYMBOLTYPE_FLOAT:
            return new FloatSymbol();
        case ESYMBOLTYPE_PAIR:
            return new PairSymbol();
        case ESYMBOLTYPE_VECTOR:
            return new VectorSymbol();
        case ESYMBOLTYPE_QSCRIPT:
            return new QScriptSymbol();
        case ESYMBOLTYPE_ARRAY:
            return new ArraySymbol();
        case ESYMBOLTYPE_STRUCTURE:
            return new StructureSymbol();
    }

    printf("Failed to resolve symbol type: %d\n", token);
    assert(false);
    return nullptr;
}
void QSymbol::ReadSymbolsFromArray(IStream *stream, uint8_t type, bool is_reference, uint32_t num_items, QSymbol **output_tokens) {

    if(num_items == 0) {
        uint32_t v = stream->ReadUInt32(); //skip null data
        assert(v == 0);
        return;
    }
    int readmode = 0;
    
    if(type == ESYMBOLTYPE_INTEGER || type == ESYMBOLTYPE_FLOAT || type == ESYMBOLTYPE_NAME || is_reference) {
        readmode = 1;
    } else if(type == ESYMBOLTYPE_STRUCTURE || type == ESYMBOLTYPE_ARRAY || type == ESYMBOLTYPE_STRING || type == ESYMBOLTYPE_LOCALSTRING || type == ESYMBOLTYPE_PAIR || type == ESYMBOLTYPE_VECTOR) {
        //probably vector and pair go into here too... but let them asert for now
        readmode = 2;
    }

    if(readmode == 1) {
        if(num_items > 1) {
            uint32_t data_offset = stream->ReadUInt32();
            stream->SetCursor(data_offset);
        }
        for(int i=0;i<num_items;i++) {
            if(is_reference) {
                output_tokens[i] = new ReferenceItemSymbol(type);
            } else {
                output_tokens[i] = Resolve(type);
            }            
            output_tokens[i]->LoadParamsFromArray(stream);
        }
    } else if(readmode == 2) {
        if(num_items > 1) {
            uint32_t data_offset = stream->ReadUInt32();
            stream->SetCursor(data_offset);
        }

        uint32_t *offsets = new uint32_t[num_items];
        for(int i=0;i<num_items;i++) {
            offsets[i] = stream->ReadUInt32();
        }
        for(int i=0;i<num_items;i++) {
            stream->SetCursor(offsets[i]);
            if(is_reference) {
                output_tokens[i] = new ReferenceItemSymbol(type);
            } else {
                output_tokens[i] = Resolve(type);
            }            
            output_tokens[i]->LoadParamsFromArray(stream);
        }
        delete offsets;
    } else {
        assert(false);
    }
}
void QSymbol::WriteSymbolsToArray(IStream *stream, uint8_t type, bool is_reference, uint32_t num_items, QSymbol **output_tokens) {
    if(num_items == 0) {
        stream->WriteUInt32(0);
        return;
    }
    int writemode = 0;
    
    if(type == ESYMBOLTYPE_INTEGER || type == ESYMBOLTYPE_FLOAT || type == ESYMBOLTYPE_NAME || is_reference) {
        writemode = 1;
    } else if(type == ESYMBOLTYPE_STRUCTURE || type == ESYMBOLTYPE_ARRAY || type == ESYMBOLTYPE_STRING || type == ESYMBOLTYPE_LOCALSTRING || type == ESYMBOLTYPE_PAIR || type == ESYMBOLTYPE_VECTOR) {
        //probably vector and pair go into here too... but let them asert for now
        writemode = 2;
    }


   if(writemode == 1) {
        if(num_items > 1) {
            uint32_t data_offset = stream->GetOffset() + sizeof(uint32_t);
            stream->WriteUInt32(data_offset);
        }
        for(int i=0;i<num_items;i++) {
                assert(output_tokens[i]);
                output_tokens[i]->WriteToArray(stream);
        }
   } else if(writemode == 2) {
        if(num_items > 1) {
            uint32_t data_offset = stream->GetOffset() + sizeof(uint32_t);
            stream->WriteUInt32(data_offset);
        }
        size_t *item_offsets = new size_t[num_items];
        uint32_t *offsets = new uint32_t[num_items];
        for(int i=0;i<num_items;i++) {
            item_offsets[i] = stream->GetOffset();
            stream->WriteUInt32(0);
        }
        for(int i=0;i<num_items;i++) {
            if(is_reference) {
                assert(false);
            } else {
                offsets[i] = stream->GetOffset();
                if(output_tokens[i]->GetIsStructItem()) {
                    offsets[i] += sizeof(uint32_t);
                }
                output_tokens[i]->WriteToArray(stream);
            }
        }
        size_t cursor = stream->GetOffset();
        for(int i=0;i<num_items;i++) {
            stream->SetCursor(item_offsets[i]);
            stream->WriteUInt32(offsets[i]); //write data pointer
        }
        stream->SetCursor(cursor);

        delete[] item_offsets;
        delete[] offsets;
   }

}
void QSymbol::LoadParamsFromArray(IStream *stream) {
    assert(false);
}
uint32_t QSymbol::GetNextOffset() {
    return m_next_offset;
}