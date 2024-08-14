
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

#include <ArraySymbol.h>

QSymbolToken *ConvertToken(QScriptToken *token) {
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
    }
    return nullptr;
}
void emit_symbol() {
    QScriptToken *token = g_Deopt.currentToken;
    QSymbolToken *converted = ConvertToken(token);
    assert(converted);
    g_Deopt.write_stream->WriteSymbol(converted);
    delete converted;
}

void emit_script() {
    const int TOKEN_BUFF_SIZE = 1024;
    uint8_t *token_buffer = new uint8_t[TOKEN_BUFF_SIZE];

    MemoryStream ms(token_buffer, TOKEN_BUFF_SIZE);
    std::vector<QScriptToken *>::iterator it = g_Deopt.script_tokens.begin();
    while(it != g_Deopt.script_tokens.end()) {
        QScriptToken *token = *it;
        token->Write(&ms);
        it++;
    }

    QScriptSymbol *symbol = new QScriptSymbol(token_buffer, ms.GetOffset());
    symbol->SetNameChecksum(g_Deopt.root_name_checksum);
    g_Deopt.write_stream->WriteSymbol(symbol);
    delete symbol;

    g_Deopt.script_tokens.clear();
    
}

void emit_array() {
    if(g_Deopt.script_tokens.empty()) { //TODO: handle empty array
        assert(false);
    }

    switch(g_Deopt.script_tokens.front()->GetType()) {
            case ESCRIPTTOKEN_STARTSTRUCT:
            printf("emit array of structs\n");
            return;
            case ESCRIPTTOKEN_STARTARRAY:
            printf("emit array of arrays\n");
            return;
    }
        
    size_t num_tokens = g_Deopt.script_tokens.size();
    QSymbolToken **sym_tokens = new QSymbolToken*[num_tokens];

    for(int i=0;i<num_tokens;i++) {
        QScriptToken *t = g_Deopt.script_tokens.at(i);

        sym_tokens[i] = ConvertToken(t);
        assert(sym_tokens[i]);
    }


    ArraySymbol sym(sym_tokens, num_tokens);
    sym.SetNameChecksum(g_Deopt.root_name_checksum);
    g_Deopt.write_stream->WriteSymbol(&sym);

    delete[] sym_tokens;

    g_Deopt.script_tokens.clear();


}
