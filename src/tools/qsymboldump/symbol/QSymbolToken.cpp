#include "QSymbolToken.h"
#include <cassert>

#include "IntegerSymbol.h"
#include "NameSymbol.h"
#include "FloatSymbol.h"
#include "ArraySymbol.h"
#include "StructureSymbol.h"
#include "QScriptSymbol.h"
QSymbolToken::QSymbolToken() {

}
QSymbolToken::~QSymbolToken() {

}

QSymbolToken* QSymbolToken::Resolve(uint8_t token) {
    switch(token) {
        case ESYMBOLTYPE_INTEGER:
            return new IntegerSymbol();
        case ESYMBOLTYPE_NAME:
            return new NameSymbol();
        case ESYMBOLTYPE_FLOAT:
            return new FloatSymbol();
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