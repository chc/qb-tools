#include <stdio.h>
#include <stdint.h>
#include "qopt.h"
#include <QStream.h>
#include <QScriptToken.h>
#include <NameToken.h>
#include <IntegerToken.h>
#include <FloatToken.h>
#include <ArgumentPackToken.h>

#include <SymbolFileStream.h>
#include <cassert>
#include <crc32.h>

QOpt g_QOpt;


void handle_read_token_value(QScriptToken *token) {
    g_QOpt.currentState = QOptState_ReadNextGlobalToken;
    g_QOpt.currentToken = token;
    switch(token->GetType()) {
        case ESCRIPTTOKEN_ARGUMENTPACK:
            g_QOpt.in_argument_pack = true;
            g_QOpt.last_argpack = reinterpret_cast<ArgumentPackToken*>(token);
            g_QOpt.currentState = QOptState_ReadTokenValue;
        break;
        case ESCRIPTTOKEN_INTEGER:
        case ESCRIPTTOKEN_FLOAT:
        case ESCRIPTTOKEN_NAME:
        case ESCRIPTTOKEN_STRING:
        case ESCRIPTTOKEN_LOCALSTRING:
        case ESCRIPTTOKEN_PAIR:
        case ESCRIPTTOKEN_VECTOR:
            emit_symbol();
            g_QOpt.in_argument_pack = false;
            break;
        case ESCRIPTTOKEN_STARTARRAY:
            g_QOpt.currentState = QOptState_ReadArrayTokens;
            g_QOpt.depth_index = 1;
            //printf("root start array\n");
            break;
        case ESCRIPTTOKEN_STARTSTRUCT:
            g_QOpt.currentState = QOptState_ReadStructTokens;
            g_QOpt.depth_index = 1;
            g_QOpt.script_tokens.push_back(token);
            //printf("root start struct\n");
            break;
        case ESCRIPTTOKEN_ENDOFLINE:
        case ESCRIPTTOKEN_ENDOFLINENUMBER:
            break;
        default:
            assert(false);
    }
}

void handle_global_token_state(QScriptToken *token) {
    switch(token->GetType()) {
        case ESCRIPTTOKEN_NAME:
            g_QOpt.root_name_checksum = reinterpret_cast<NameToken*>(token)->GetChecksum();           
            g_QOpt.currentState = QOptState_ReadEqualsToken;
        break;
        case ESCRIPTTOKEN_KEYWORD_SCRIPT:
            g_QOpt.currentState = QOptState_ReadScriptName;
            break;
        break;
        case ESCRIPTTOKEN_INTEGER:
            //in THAW PC tod_manager.qb there is an unnamed integer token with a value of 0... 
            // this is the only known case where this happens so far, global symbols with no name are likely of no use, so just skip
            g_QOpt.currentState = QOptState_ReadNextGlobalToken;
            assert(reinterpret_cast<IntegerToken*>(token)->GetValue() == 0);
        break;
        default:
            assert(false);
        case ESCRIPTTOKEN_CHECKSUM_NAME:
        case ESCRIPTTOKEN_ENDOFLINE:
        case ESCRIPTTOKEN_ENDOFLINENUMBER:
            break;
    }
}

void handle_read_script_name(QScriptToken *token) {
        switch(token->GetType()) {
            case ESCRIPTTOKEN_NAME:
                g_QOpt.root_name_checksum = reinterpret_cast<NameToken*>(token)->GetChecksum();
                g_QOpt.currentState = QOptState_ReadScriptTokens;
            break;
            default:
                assert(false);
        }
}


void handle_equals_token_state(QScriptToken *token) {
    switch(token->GetType()) {
        case ESCRIPTTOKEN_EQUALS:
            g_QOpt.currentState = QOptState_ReadTokenValue;
        break;
        //ignore
        case ESCRIPTTOKEN_ENDOFLINE:
        case ESCRIPTTOKEN_ENDOFLINENUMBER:
            break;
        default:
            assert(false);
    }
}
void handle_read_root_array(QScriptToken *token) {
    if(token->GetType() == ESCRIPTTOKEN_ENDARRAY) {
        g_QOpt.depth_index--;
        if(g_QOpt.depth_index > 0) {
            g_QOpt.script_tokens.push_back(token);
        }
    } else if(token->GetType() == ESCRIPTTOKEN_STARTARRAY) {
        g_QOpt.depth_index++;
        g_QOpt.script_tokens.push_back(token);
    } else if(token->GetType() == ESCRIPTTOKEN_ENDOFLINE || token->GetType() == ESCRIPTTOKEN_ENDOFLINENUMBER) {

    }
    else {
        g_QOpt.script_tokens.push_back(token);
    }

    if(g_QOpt.depth_index == 0) {
        //printf("Got end of array, %08x\n", g_QOpt.root_name_checksum);
        emit_array();
        g_QOpt.script_tokens.clear();
        g_QOpt.currentState = QOptState_ReadNextGlobalToken;
    }
}
void handle_read_root_struct(QScriptToken *token) {
    if(token->GetType() == ESCRIPTTOKEN_ENDSTRUCT) {
        g_QOpt.depth_index--;
    } else if(token->GetType() == ESCRIPTTOKEN_STARTSTRUCT) {
        g_QOpt.depth_index++;
    }
    g_QOpt.script_tokens.push_back(token);

    if(g_QOpt.depth_index == 0) {
        //printf("Got end of struct, %08x\n", g_QOpt.root_name_checksum);
        emit_struct();
        g_QOpt.currentState = QOptState_ReadNextGlobalToken;
    }
}
void handle_read_script_tokens(QScriptToken *token) {
    g_QOpt.script_tokens.push_back(token);
    switch(token->GetType()) {
        case ESCRIPTTOKEN_KEYWORD_ENDSCRIPT:
        emit_script();
        g_QOpt.currentState = QOptState_ReadNextGlobalToken;
        break;
    }
}

int main(int argc, const char *argv[]) {
    if(argc  < 3) {
        fprintf(stderr, "usage: %s [in] [out]\n",argv[0]);
        return -1;
    }
    g_QOpt.currentState = QOptState_ReadNextGlobalToken;
    g_QOpt.depth_index = 0;
    g_QOpt.in_argument_pack = false;
    FileStream fs(argv[1]);

    QStream qs = QStream(&fs);

    if(!fs.IsFileOpened()) {
        fprintf(stderr, "Failed to open file: %s\n", argv[1]);
        return -1;
    }

    FileStream out_fs(argv[2], true);
    if(!out_fs.IsFileOpened()) {
        fprintf(stderr, "Failed to open file: %s\n", argv[2]);
        return -1;
    }

    out_fs.SetWriteEndian(ISTREAM_SYMBOL_ENDIAN);
    SymbolFileStream sym_fs(&out_fs);
    uint32_t src_checksum_name = thps_gen_checksum(argv[1]);
    sym_fs.SetSourceChecksum(src_checksum_name);

    sym_fs.WriteHeader();

    g_QOpt.write_stream = reinterpret_cast<ISStream *>(&sym_fs);

    QScriptToken *token;
    while(true) {
        uint32_t offset = fs.GetOffset();
        token = qs.NextToken();
        if(token == NULL || token->GetType() == ESCRIPTTOKEN_ENDOFFILE) {
            break;
        }
        token->SetFileOffset(offset);
        if(token->GetType() == ESCRIPTTOKEN_ARGUMENTPACK) {
            ArgumentPackToken *argpack = reinterpret_cast<ArgumentPackToken*>(token);
            argpack->LoadExtendedParams(&fs);
        }
        switch(g_QOpt.currentState) {
            case QOptState_ReadNextGlobalToken:
                handle_global_token_state(token);
                break;
            case QOptState_ReadEqualsToken:
                handle_equals_token_state(token);
                break;
            case QOptState_ReadTokenValue:
                handle_read_token_value(token);
                break;
            case QOptState_ReadScriptName:
                handle_read_script_name(token);
                break;
            case QOptState_ReadScriptTokens:
                handle_read_script_tokens(token);
                break;
            case QOptState_ReadArrayTokens:
                handle_read_root_array(token);
                break;
            case QOptState_ReadStructTokens:
                handle_read_root_struct(token);
                break;
        }
    }

    sym_fs.UpdateHeaderSize();

    return 0;
}