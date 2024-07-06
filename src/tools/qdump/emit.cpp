
#include <stdio.h>
#include <stdint.h>
#include <cassert>
#include <string.h>

#include "main.h"
#include "Token.h"


void EmitToken(EScriptToken token, void *token_data, size_t token_len) {
    float* float_v = (float *)token_data;
    switch(token) {
        case ESCRIPTTOKEN_NAME:
            fprintf(g_DumpState.fd_out, "$%ud ", *(uint32_t*)token_data);
        break;
        case ESCRIPTTOKEN_EQUALS:
            fprintf(g_DumpState.fd_out, "= ");
        break;
        case ESCRIPTTOKEN_INTEGER:
            fprintf(g_DumpState.fd_out, "%d ", *(int32_t*)token_data);
        break;
        case ESCRIPTTOKEN_FLOAT:
            fprintf(g_DumpState.fd_out, "%f ", *(float*)token_data);
        break;
        case ESCRIPTTOKEN_STRING:
        fprintf(g_DumpState.fd_out, "\"%s\" ", token_data);
        break;
        case ESCRIPTTOKEN_KEYWORD_IF:
        fprintf(g_DumpState.fd_out, "if ");
        break;
        case ESCRIPTTOKEN_KEYWORD_ELSE:
        fprintf(g_DumpState.fd_out, "else");
        break;
        case ESCRIPTTOKEN_KEYWORD_NOT:
        fprintf(g_DumpState.fd_out, "not ");
        break;
        case ESCRIPTTOKEN_KEYWORD_ENDIF:
        fprintf(g_DumpState.fd_out, "endif");
        break;
        case ESCRIPTTOKEN_KEYWORD_SCRIPT:
        fprintf(g_DumpState.fd_out, "script ");
        break;
        case ESCRIPTTOKEN_KEYWORD_ENDSCRIPT:
        fprintf(g_DumpState.fd_out, "endscript");
        break;
        case ESCRIPTTOKEN_STARTSTRUCT:
        fprintf(g_DumpState.fd_out, "{");
        break;
        case ESCRIPTTOKEN_ENDSTRUCT:
        fprintf(g_DumpState.fd_out, "}");
        break;
        case ESCRIPTTOKEN_STARTARRAY:
        fprintf(g_DumpState.fd_out, "[");
        break;
        case ESCRIPTTOKEN_ENDARRAY:
        fprintf(g_DumpState.fd_out, "]");
        break;
        case ESCRIPTTOKEN_OPENPARENTH:
        fprintf(g_DumpState.fd_out, ")");
        break;
        case ESCRIPTTOKEN_CLOSEPARENTH:
        fprintf(g_DumpState.fd_out, "(");
        break;
        case ESCRIPTTOKEN_PAIR:
        fprintf(g_DumpState.fd_out, "(%f,%f) ", *float_v, *(float_v+1));
        break;
        /*case ESCRIPTTOKEN_ENDOFLINE:
        fprintf(g_DumpState.fd_out,"\n");
        break;*/
        case ESCRIPTTOKEN_ENDOFLINENUMBER:
        /*do {
            fprintf(g_DumpState.fd_out, "\n");
        } while(QDumpState.last_line_number++ != *(int32_t*)token_data);*/
        fprintf(g_DumpState.fd_out, "\n");
        
        break;
        case ESCRIPTTOKEN_ARG:
        fprintf(g_DumpState.fd_out, "ARG "); //XXX: handle this
        break;
        case ESCRIPTTOKEN_KEYWORD_ALLARGS:
        fprintf(g_DumpState.fd_out, "<...>");
        break;
        case ESCRIPTTOKEN_KEYWORD_BEGIN:
        fprintf(g_DumpState.fd_out, "begin");
        break;
        case ESCRIPTTOKEN_KEYWORD_REPEAT:
        fprintf(g_DumpState.fd_out, "repeat");
        break;
        case ESCRIPTTOKEN_CHECKSUM_NAME:
        break;
        default:
            printf("Token Type: %02d - %s\n", token, GetTokenName(token));
            if(token_data != NULL) {
                show_dump((unsigned char *)token_data, token_len, stderr);
            }
            assert(false);
        break;
    }
}