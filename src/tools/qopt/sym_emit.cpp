
#include <cassert>
#include <MemoryStream.h>

#include "qopt.h"
#include <QSymbolToken.h>
#include <QScriptToken.h>
#include <QScriptSymbol.h>

#include <NameToken.h>
#include <NameSymbol.h>
#include <IntegerToken.h>
#include <IntegerSymbol.h>
#include <FloatToken.h>
#include <FloatSymbol.h>
#include <StringSymbol.h>
#include <StringToken.h>

#include <ArraySymbol.h>
#include <LocalStringToken.h>
#include <LocalStringSymbol.h>
#include <ReferenceItemSymbol.h>
QSymbolToken *ConvertToken(QScriptToken *token, bool make_reference) {
    if(make_reference) {      
        assert(token->GetType() == ESCRIPTTOKEN_NAME);
        ReferenceItemSymbol *ref = new ReferenceItemSymbol(ESYMBOLTYPE_STRUCTURE);
        //for now refs always point to structs... maybe it works this way?
        NameToken *name = reinterpret_cast<NameToken *>(token);
        ref->SetValue(name->GetChecksum());
        return ref;
    }
    switch(token->GetType()) {
        case ESCRIPTTOKEN_INTEGER:
            return new IntegerSymbol(reinterpret_cast<IntegerToken*>(token)->GetValue());
            break;
        case ESCRIPTTOKEN_FLOAT:
            return new FloatSymbol(reinterpret_cast<FloatToken*>(token)->GetValue());
            break;
        case ESCRIPTTOKEN_NAME:
            return new NameSymbol(reinterpret_cast<NameToken*>(token)->GetChecksum());
            break;
        case ESCRIPTTOKEN_STRING:
            return new StringSymbol(reinterpret_cast<StringToken*>(token)->GetValue());
            break;
        case ESCRIPTTOKEN_LOCALSTRING:
            return new LocalStringSymbol(reinterpret_cast<LocalStringToken*>(token)->GetValue());
            break;
    }
    assert(false);
    return nullptr;
}
void emit_symbol() {
    QScriptToken *token = g_Deopt.currentToken;
    QSymbolToken *converted = ConvertToken(token);
    assert(converted);
    converted->SetNameChecksum(g_Deopt.root_name_checksum);
    g_Deopt.write_stream->WriteSymbol(converted);
    delete converted;
}



