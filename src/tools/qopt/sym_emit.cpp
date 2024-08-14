
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

template<class T>
void emit_token_value(T symbol) {
    g_Deopt.write_stream->WriteSymbol(&symbol);
}
void emit_symbol() {
    QScriptToken *token = g_Deopt.currentToken;
    switch(token->GetType()) {
        case ESCRIPTTOKEN_INTEGER:
            emit_token_value(IntegerSymbol(reinterpret_cast<IntegerToken*>(token)->GetValue()));
            break;
        case ESCRIPTTOKEN_FLOAT:
            emit_token_value(FloatSymbol(reinterpret_cast<FloatToken*>(token)->GetValue()));
            break;
        case ESCRIPTTOKEN_NAME:
            emit_token_value(FloatSymbol(reinterpret_cast<FloatToken*>(token)->GetValue()));
            break;
        default:
            assert(false);
    }
}

void emit_script() {
    printf("emit script: %08x\n", g_Deopt.root_name_checksum);
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
    printf("emitted\n");
    
}