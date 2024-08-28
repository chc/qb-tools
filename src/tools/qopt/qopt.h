#ifndef _QOPT_H
#define _QOPT_H

#include <stdio.h>
#include <stdint.h>
#include <QScriptToken.h>
#include <ISStream.h>
#include <vector>

enum QOptState {
    QOptState_ReadNextGlobalToken,
    QOptState_ReadEqualsToken,
    QOptState_ReadTokenValue,
    QOptState_ReadScriptName,
    QOptState_ReadScriptTokens,
    QOptState_ReadArrayTokens,
    QOptState_ReadStructTokens,
};

class ArgumentPackToken;
typedef struct {
    QOptState currentState;
    uint32_t source_checksum;
    uint32_t root_name_checksum;
    uint32_t depth_index;
    bool in_argument_pack;
    ArgumentPackToken *last_argpack;

    QScriptToken *currentToken;
    ISStream *write_stream;

    std::vector<QScriptToken *> script_tokens;
} QOpt;


extern QOpt g_QOpt;

void emit_symbol();
void emit_script();

void emit_struct();

void emit_array();
void emit_array_of_structs();

bool is_end_of_line_token(QScriptToken *token);

class ArgumentPackToken;
QSymbol *ConvertToken(QScriptToken *token, bool make_reference = false, ArgumentPackToken *last_argpack = NULL);

#endif //_QOPT_H