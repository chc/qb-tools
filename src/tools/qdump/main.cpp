
#include <stdio.h>
#include <stdint.h>
#include <cassert>
#include <string.h>
#include "main.h"

#include "Token.h"

QDumpState g_DumpState;


const char *dump_path = "/Users/chc/skate5/Data/scripts/game/startup.qb";

void EmitToken(EScriptToken token, void *token_data, size_t token_len);

void show_dump(unsigned char *data, unsigned int len, FILE *stream);

size_t ReadChecksumName(void **output) {

    const size_t MAX_CHECKSUM_NAME = sizeof(uint32_t) + 256;
    char checksum_data[MAX_CHECKSUM_NAME];
    char *p = (char *)&checksum_data[0];

    size_t result = 0;

    result += sizeof(uint32_t);

    fread(p, sizeof(uint32_t), 1, g_DumpState.fd_in);
    p += sizeof(uint32_t);

    
    do {
        int r = fread(p, sizeof(uint8_t), 1, g_DumpState.fd_in);
        if(r == 0) break;
        result++;
    }   while(*p++);


    if(output != NULL) {
        *output = malloc(result);
        memcpy(*output, &checksum_data, result);
    }
    return result;
}
size_t GetTokenData(EScriptToken token, void **output) {

    size_t data_len = 0;
    switch(token) {
        case ESCRIPTTOKEN_NAME:
        case ESCRIPTTOKEN_INTEGER:
        case ESCRIPTTOKEN_FLOAT:
        case ESCRIPTTOKEN_ENDOFLINENUMBER:
            data_len = sizeof(uint32_t);
        break;
        case ESCRIPTTOKEN_EQUALS:
        case ESCRIPTTOKEN_KEYWORD_IF:
        case ESCRIPTTOKEN_KEYWORD_ENDIF:
        case ESCRIPTTOKEN_KEYWORD_NOT:
        case ESCRIPTTOKEN_KEYWORD_ENDSCRIPT:
        case ESCRIPTTOKEN_KEYWORD_ELSE:
        case ESCRIPTTOKEN_KEYWORD_ALLARGS:
        case ESCRIPTTOKEN_STARTARRAY:
        case ESCRIPTTOKEN_ENDARRAY:
        case ESCRIPTTOKEN_STARTSTRUCT:
        case ESCRIPTTOKEN_ENDSTRUCT:
        case ESCRIPTTOKEN_OPENPARENTH:
        case ESCRIPTTOKEN_CLOSEPARENTH:
        case ESCRIPTTOKEN_ARG:
        case ESCRIPTTOKEN_KEYWORD_BEGIN:
        case ESCRIPTTOKEN_KEYWORD_REPEAT:
        break;
        case ESCRIPTTOKEN_CHECKSUM_NAME:
            return ReadChecksumName(output);
        break;
        case ESCRIPTTOKEN_PAIR:
            data_len = sizeof(uint32_t) + sizeof(uint32_t);
        break;
        case ESCRIPTTOKEN_STRING:
            uint32_t string_len;
            fread(&string_len, sizeof(uint32_t), 1, g_DumpState.fd_in);
            data_len = string_len;
        break;
        case ESCRIPTTOKEN_KEYWORD_SCRIPT:
        break;
        default:
        assert(false);
    }

    if(data_len > 0) {
        *output = malloc(data_len);
        fread(*output, data_len, 1, g_DumpState.fd_in);
    }
    return data_len;
}



EScriptToken GetNextToken() {
    EScriptTokenType type;
    size_t len = fread(&type, sizeof(EScriptTokenType),1,g_DumpState.fd_in);

    if(len != sizeof(EScriptTokenType)) {
        return ESCRIPTTOKEN_ENDOFFILE;
    }
   return (EScriptToken)type;
}



int main() {
    //memset(&QDumpState, 0, sizeof(QDumpState));
    //QDumpState.last_line_number = 1;
    //QDumpState.tab_index = 1;

    g_DumpState.fd_in = fopen(dump_path, "rb");
    g_DumpState.fd_out = stdout;
    if(!g_DumpState.fd_in) {
        printf("Failed to open dump file!\n");
        return 0;
    }
    EScriptToken token;
    while( (token = GetNextToken()) != ESCRIPTTOKEN_ENDOFFILE) {
       
        void *token_data = NULL;
        size_t len = GetTokenData(token, &token_data);

        EmitToken(token, token_data, len);
        
        if(token_data != NULL) {
            //show_dump((unsigned char *)token_data, len, stderr);
            free(token_data);
        }
    }

    fclose(g_DumpState.fd_in);
    return 0;
}