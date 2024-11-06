
#include <cassert>
#include <MemoryStream.h>

#include "qopt.h"
#include <QSymbol.h>
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
#include <PairToken.h>
#include <PairSymbol.h>
#include <VectorToken.h>
#include <VectorSymbol.h>
#include <ArgumentPackToken.h>
QSymbol *ConvertToken(QScriptToken *token, bool make_reference, ArgumentPackToken *last_argpack) {
    if(make_reference) {      
        assert(token->GetType() == ESCRIPTTOKEN_NAME);
        assert(last_argpack);
        ReferenceItemSymbol *ref = new ReferenceItemSymbol(last_argpack->GetRefType(), last_argpack->GetIsRequiredParams());
        //for now refs always point to structs... maybe it works this way?
        NameToken *name = reinterpret_cast<NameToken *>(token);
        ref->SetValue(name->GetChecksum());
        return ref;
    }

    PairToken *pt;
    VectorToken *vt;
    switch(token->GetType()) {
        case ESCRIPTTOKEN_INTEGER:
            return new IntegerSymbol(reinterpret_cast<IntegerToken*>(token)->GetValue());
        case ESCRIPTTOKEN_FLOAT:
            return new FloatSymbol(reinterpret_cast<FloatToken*>(token)->GetValue());
        case ESCRIPTTOKEN_NAME:
            return new NameSymbol(reinterpret_cast<NameToken*>(token)->GetChecksum());
        case ESCRIPTTOKEN_STRING:
            return new StringSymbol(reinterpret_cast<StringToken*>(token)->GetValue());
        case ESCRIPTTOKEN_LOCALSTRING:
            return new LocalStringSymbol(reinterpret_cast<LocalStringToken*>(token)->GetValue());
        case ESCRIPTTOKEN_PAIR:
            pt = reinterpret_cast<PairToken*>(token);
            return new PairSymbol(pt->GetX(), pt->GetY());
        case ESCRIPTTOKEN_VECTOR:
            vt = reinterpret_cast<VectorToken*>(token);
            return new VectorSymbol(vt->GetX(), vt->GetY(), vt->GetZ());
            
    }
    assert(false);
    return nullptr;
}
void emit_symbol() {
    QScriptToken *token = g_QOpt.currentToken;
    QSymbol *converted = ConvertToken(token, g_QOpt.in_argument_pack, g_QOpt.last_argpack);
    assert(converted);
    converted->SetNameChecksum(g_QOpt.root_name_checksum);
    converted->SetIsStructItem(true);
    g_QOpt.write_stream->WriteSymbol(converted);
    delete converted;
}


bool is_end_of_line_token(QScriptToken *token) {
    switch(token->GetType()) {
        case ESCRIPTTOKEN_ENDOFLINE:
        case ESCRIPTTOKEN_ENDOFLINENUMBER:
        case ESCRIPTTOKEN_ENDOFFILE:
            return true;
    }
    return false;
}