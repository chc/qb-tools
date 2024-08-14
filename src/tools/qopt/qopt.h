#ifndef _QOPT_H
#define _QOPT_H

#include <stdio.h>
#include <stdint.h>
#include <QScriptToken.h>
#include <ISStream.h>
#include <vector>

enum DeoptState {
    DeoptState_ReadNextGlobalToken,
    DeoptState_ReadEqualsToken,
    DeoptState_ReadTokenValue,
    DeoptState_ReadScriptName,
    DeoptState_ReadScriptTokens,
    DeoptState_ReadArrayTokens,
    DeoptState_ReadStructTokens,
};


typedef struct {
    DeoptState currentState;
    uint32_t source_checksum;
    uint32_t root_name_checksum;
    uint32_t depth_index;

    QScriptToken *currentToken;
    ISStream *write_stream;

    std::vector<QScriptToken *> script_tokens;
} Deopt;


extern Deopt g_Deopt;

void emit_symbol();
void emit_script();
void emit_array();

#endif //_QOPT_H