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
void QSymbolToken::LoadParamsFromArray(IStream *stream) {
    assert(false);
}
uint32_t QSymbolToken::GetNextOffset() {
    return m_next_offset;
}