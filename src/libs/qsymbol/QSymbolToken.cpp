#include "QSymbolToken.h"
#include <cassert>

#include "IntegerSymbol.h"
#include "NameSymbol.h"
#include "LocalStringSymbol.h"
#include "StringSymbol.h"
#include "FloatSymbol.h"
#include "VectorSymbol.h"
#include "ArraySymbol.h"
#include "StructureSymbol.h"
#include "QScriptSymbol.h"
QSymbolToken::QSymbolToken() {
    m_next_offset = 0;
    m_name_checksum = 0;
    m_source_checksum = 0;
    m_struct_item = false;

}
QSymbolToken::~QSymbolToken() {

}

QSymbolToken* QSymbolToken::Resolve(uint8_t token) {
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
void QSymbolToken::ReadSymbolsFromArray(IStream *stream, uint8_t type, uint32_t num_items, QSymbolToken **output_tokens) {
    int readmode = 0;
    if(type == ESYMBOLTYPE_INTEGER || type == ESYMBOLTYPE_FLOAT || type == ESYMBOLTYPE_NAME) {
        readmode = 1;
    } else if(type == ESYMBOLTYPE_STRUCTURE || type == ESYMBOLTYPE_ARRAY || type == ESYMBOLTYPE_STRING || type == ESYMBOLTYPE_LOCALSTRING) {
        //probably vector and pair go into here too... but let them asert for now
        readmode = 2;
    }

    if(readmode == 1) {
        for(int i=0;i<num_items;i++) {
            output_tokens[i] = Resolve(type);
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
            output_tokens[i] = Resolve(type);
            output_tokens[i]->LoadParamsFromArray(stream);
        }
        delete offsets;
    } else {
        assert(false);
    }
}
void QSymbolToken::LoadParamsFromArray(IStream *stream) {
    assert(false);
}
uint32_t QSymbolToken::GetNextOffset() {
    return m_next_offset;
}