#include "qcomp.h"

bool handle_keyword_check(std::string token, FileStream &fs_out) {
    if (token.compare("script") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_SCRIPT, fs_out);
    } else if (token.compare("endscript") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_ENDSCRIPT, fs_out);        
    } else if(token.compare("if") == 0) {
        #ifdef ENABLE_NEWIFS
            emit_token(ESCRIPTTOKEN_KEYWORD_FASTIF, fs_out);
        #else
            emit_token(ESCRIPTTOKEN_KEYWORD_IF, fs_out);
        #endif        
    } else if(token.compare("elseif") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_ELSEIF, fs_out);
    } else if(token.compare("else") == 0) {
        #ifdef ENABLE_NEWIFS
            emit_token(ESCRIPTTOKEN_KEYWORD_FASTELSE, fs_out);
        #else
            emit_token(ESCRIPTTOKEN_KEYWORD_ELSE, fs_out);
        #endif    
    } else if(token.compare("endif") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_ENDIF, fs_out);       
    }else if(token.compare("switch") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_SWITCH, fs_out);       
    }else if(token.compare("endswitch") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_ENDSWITCH, fs_out);       
    }else if(token.compare("case") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_CASE, fs_out);       
    }else if(token.compare("default") == 0 && g_QCompState.case_count > 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_DEFAULT, fs_out);       
    } else if(token.compare("NOT") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_NOT, fs_out);
    } else if(token.compare("repeat") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_REPEAT, fs_out);
    } else if(token.compare("begin") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_BEGIN, fs_out);
    } else if(token.compare(">=") == 0) {
        emit_token(ESCRIPTTOKEN_GREATERTHANEQUAL, fs_out);
    } else if(token.compare("<=") == 0) {
        emit_token(ESCRIPTTOKEN_LESSTHANEQUAL, fs_out);
    } 
#ifdef ENABLE_NOTEQUAL
    else if(token.compare("!=") == 0) {
        emit_token(ESCRIPTTOKEN_NOTEQUAL, fs_out);
    } 
#endif
#ifdef ENABLE_SQS_TOKENS
    else if(token.compare(0, 3, "SQS") == 0 && g_QCompState.emit_type == ESCRIPTTOKEN_OPENPARENTH) {
        g_QCompState.read_mode = EReadMode_ReadSQSToken;
        assert(g_QCompState.emit_type == ESCRIPTTOKEN_OPENPARENTH);
        g_QCompState.emit_type = 0;
        g_QCompState.temp_token = token + "("; //left over ( is still processed later, will need to clear before changing states again too
    }
#endif
     else if(token.compare("return") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_RETURN, fs_out);
    } else if(token.compare("break") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_BREAK, fs_out);
    } else if(token.compare("AND") == 0) {
        emit_token(ESCRIPTTOKEN_AND, fs_out);
    } else if(token.compare("OR") == 0) {
        emit_token(ESCRIPTTOKEN_OR, fs_out);
    } else if(token.compare("<...>") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_ALLARGS, fs_out);
    } else if(token.compare("<") == 0) {
        emit_token(ESCRIPTTOKEN_LESSTHAN, fs_out);
    } else if(token.compare(">") == 0) {
        emit_token(ESCRIPTTOKEN_GREATERTHAN, fs_out);
    } else if(token.compare("=") == 0) {
        emit_token(ESCRIPTTOKEN_EQUALS, fs_out);
    } else if(token.compare("(") == 0) {
        emit_token(ESCRIPTTOKEN_OPENPARENTH, fs_out);
    } else if(token.compare(")") == 0) {
        emit_token(ESCRIPTTOKEN_CLOSEPARENTH, fs_out);
    } else if(token.compare(0, 4, "Pair") == 0 || token.compare(0, 3, "Vec") == 0 && g_QCompState.emit_type == ESCRIPTTOKEN_OPENPARENTH) {
        g_QCompState.read_mode = EReadMode_ReadPairOrVector;
        assert(g_QCompState.emit_type == ESCRIPTTOKEN_OPENPARENTH);
        g_QCompState.emit_type = 0;
        g_QCompState.temp_token = token + "("; //left over ( is still processed later, will need to clear before changing states again too
    } 
#if QTOKEN_SUPPORT_LEVEL > 5
    else if(token.compare("RandomInt") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_RANDOMINTEGER, fs_out);
    } else if(token.compare("RandomFloat") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_RANDOMFLOAT, fs_out);
    }
#endif //QTOKEN_SUPPORT_LEVEL > 6
#if QTOKEN_SUPPORT_LEVEL > 6
    else if(token.compare("TypeHint_Controller") == 0) {
        emit_token(ESCRIPTTOKEN_TYPEHINT_CONTROLLER, fs_out);
    }
    else if(token.compare("TypeHint_ID") == 0) {
        emit_token(ESCRIPTTOKEN_TYPEHINT_ID, fs_out);
    }
    else if(token.compare("TypeHint_UserInput") == 0) {
        emit_token(ESCRIPTTOKEN_TYPEHINT_USERINPUT, fs_out);
    }
    else if(token.compare("TypeHint_Callback") == 0) {
        emit_token(ESCRIPTTOKEN_TYPEHINT_CALLBACK, fs_out);
    }
    else if(token.compare("TypeHint_Playlist") == 0) {
        emit_token(ESCRIPTTOKEN_TYPEHINT_PLAYLIST, fs_out);
    }
#endif
    else if (token.compare("RandomRange") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_RANDOM_RANGE, fs_out);
    }
    else if (token.compare("RandomPermute") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_RANDOM_PERMUTE, fs_out);
    }
    else if (token.compare("RandomNoRepeat") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_RANDOM_NO_REPEAT, fs_out);
    }
    else if (token.compare("Random") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_RANDOM, fs_out);
    }
    else {
        return false;
    }
    return true;
}
void handle_read_pair_or_vec(char ch, FileStream &fs_out) {
    bool exit_state = false;
    switch(ch) {
        case ')':
            exit_state = true;
        //break;
        default:
        g_QCompState.temp_token += ch;
        break;
    }
    if(exit_state) {
        g_QCompState.read_mode = EReadMode_ReadName;
        emit_pair_or_vec(g_QCompState.temp_token, fs_out);
        g_QCompState.temp_token.clear();
        g_QCompState.current_token.clear();
    }
}

void handle_read_name(char ch, FileStream &fs_out) {
    bool skip_token = false;
    switch(ch) {
        case '#':
            g_QCompState.read_mode = EReadMode_ReadString;
            g_QCompState.emit_type = ESCRIPTTOKEN_NAME;
            g_QCompState.read_index = 0;
            return;
        break;
#ifdef ENABLE_ARGPACK
        case '$':
            skip_token = true;
            g_QCompState.read_mode = EReadMode_ReadDollarToken;
            g_QCompState.do_arg_pack = false;
            g_QCompState.argpack_isreqparam = false;            
            g_QCompState.temp_token.clear();
            return;
        break;
#endif
        case '&':
            g_QCompState.emit_type = ESCRIPTTOKEN_ARG;
        break;
        // case '=':
        //     g_QCompState.emit_type = ESCRIPTTOKEN_EQUALS;
        // break;
        case '(':
           g_QCompState.emit_type = ESCRIPTTOKEN_OPENPARENTH;         
        break;
        case ')':
            g_QCompState.emit_type = ESCRIPTTOKEN_CLOSEPARENTH;            
        break;
        case '[':
            g_QCompState.emit_type = ESCRIPTTOKEN_STARTARRAY;
        break;
        case ']':
            g_QCompState.emit_type = ESCRIPTTOKEN_ENDARRAY;
        break;
        // case '>':
        //     g_QCompState.emit_type = ESCRIPTTOKEN_GREATERTHAN;
        // break;
        // case '<':
        //     g_QCompState.emit_type = ESCRIPTTOKEN_LESSTHAN;
        // break;
        case '*':
            if (g_QCompState.next_is_random_start_token) { //indicates this is a weight
                g_QCompState.random_read_weight = true;
                skip_token = true;
            }
            else {
                g_QCompState.emit_type = ESCRIPTTOKEN_MULTIPLY;
            }
            
        break;
        case '/':
            g_QCompState.emit_type = ESCRIPTTOKEN_DIVIDE;
        break;
        case ':':
            g_QCompState.emit_type = ESCRIPTTOKEN_COLON;
        break;
         case '.':
             if (g_QCompState.current_token.empty()) {
                 g_QCompState.emit_type = ESCRIPTTOKEN_DOT;
             }                
         break;
        case '{':
            g_QCompState.emit_type = ESCRIPTTOKEN_STARTSTRUCT;
        break;
        case '}':
            g_QCompState.emit_type = ESCRIPTTOKEN_ENDSTRUCT;
        break;
        case '+':
            g_QCompState.emit_type = ESCRIPTTOKEN_ADD;
        break;
        case ',':
            g_QCompState.emit_type = ESCRIPTTOKEN_COMMA;
        break;        
        case '-':
            g_QCompState.got_negate = true;
            return;
        break;
        case '@':
            if (g_QCompState.next_is_random_start_token) {
                g_QCompState.got_empty_random = true;
            } else {
                g_QCompState.next_is_random_start_token = true;
            }
            
            skip_token = true;
            if (!g_QCompState.skip_next_random_jump) {
                g_QCompState.emit_type = ESCRIPTTOKEN_JUMP;
            }
            g_QCompState.skip_next_random_jump = false;
        break;
        case ' ':
            if(handle_keyword_check(g_QCompState.current_token, fs_out)) {
                g_QCompState.current_token.clear();
            }
            if(g_QCompState.got_negate && g_QCompState.current_token.empty()) {
                g_QCompState.got_negate = false;
                g_QCompState.emit_type = ESCRIPTTOKEN_MINUS;
            } else {
                skip_token = true;
            }            
            break;
        case '\t':
        case '\r':
            return;
        case '\n':
            if (g_QCompState.use_eol_line_numbers) {
                g_QCompState.emit_type = ESCRIPTTOKEN_ENDOFLINENUMBER;
            }
            else {
                g_QCompState.emit_type = ESCRIPTTOKEN_ENDOFLINE;
            }            
        break;
        case '\'':
            g_QCompState.read_mode = EReadMode_ReadString;
            g_QCompState.emit_type = ESCRIPTTOKEN_LOCALSTRING;
            return;
        break;
        case '"':
            g_QCompState.read_mode = EReadMode_ReadString;
            g_QCompState.emit_type = ESCRIPTTOKEN_STRING;
            return;
        break;
    }
    if(g_QCompState.emit_type != 0 || ch == ' ') {
        if(!g_QCompState.current_token.empty()) {
            emit_token(g_QCompState.current_token, fs_out);
            g_QCompState.current_token.clear();
        }
    }
    
    if(g_QCompState.emit_type != 0) {
        emit_token(g_QCompState.emit_type, fs_out);
        g_QCompState.emit_type = 0;
        return;
    }
    if(!skip_token) {
        g_QCompState.current_token += ch;
    }
    
}


void handle_read_string(char ch, FileStream &fs_out) {
    bool end_read = false;
    if(ch == '"' && g_QCompState.emit_type == ESCRIPTTOKEN_STRING) {
        end_read = true;
    } else if(ch == '\'' && g_QCompState.emit_type == ESCRIPTTOKEN_LOCALSTRING) {
        end_read = true;
    } else if(ch == '"' && g_QCompState.emit_type == ESCRIPTTOKEN_NAME) {
        if(g_QCompState.read_index++ == 1) {
            end_read = true;
            g_QCompState.read_index = 0;
        }        
    } else {
        g_QCompState.current_token += ch;
    }

    if(end_read) {
        if(g_QCompState.emit_type == ESCRIPTTOKEN_STRING) {
            StringToken *st = new StringToken(g_QCompState.current_token);
            st->Write(&fs_out);

            append_to_random_or_delete(st);
        } else if(g_QCompState.emit_type == ESCRIPTTOKEN_LOCALSTRING) {
            LocalStringToken *lst = new LocalStringToken(g_QCompState.current_token);
            lst->Write(&fs_out);  

            append_to_random_or_delete(lst);
        } else if (g_QCompState.emit_type == ESCRIPTTOKEN_NAME) {
            uint32_t checksum = gen_checksum(g_QCompState.current_token, true);
#ifdef ENABLE_ARGPACK
            if(g_QCompState.do_arg_pack) {
            #ifdef WITH_SYMBOL_SUPPORT
                g_QCompState.do_arg_pack = false;
                ArgumentPackToken apt;
                apt.SetRefType(g_QCompState.argpack_type);
                apt.SetIsRequiredParams(g_QCompState.argpack_isreqparam);
                
                apt.WriteExtendedParams(&fs_out);
            #else
                assert(false);
            #endif
            }
#endif
            NameToken *nt = new NameToken(checksum);
            nt->Write(&fs_out);

            append_to_random_or_delete(nt);
        } else {
            assert(false);
        }
        g_QCompState.read_mode = EReadMode_ReadName;
        g_QCompState.emit_type = 0;
        g_QCompState.current_token.clear();
    }
}
#ifdef ENABLE_SQS_TOKENS
void handle_read_sqs_token(char ch, FileStream &fs_out) {
    bool exit_state = false;
    switch(ch) {
        case ')':
            exit_state = true;
        //break;
        default:
        g_QCompState.temp_token += ch;
        break;
    }
    if(exit_state) {
        g_QCompState.read_mode = EReadMode_ReadName;
        emit_sqs_token(g_QCompState.temp_token, fs_out);
        g_QCompState.temp_token.clear();
        g_QCompState.current_token.clear();
    }
}
#endif
void handle_character(char ch, FileStream &fsout) {
    if(g_QCompState.read_mode == EReadMode_ReadName) {
        handle_read_name(ch, fsout);
    } else if(g_QCompState.read_mode == EReadMode_ReadPairOrVector) {
        handle_read_pair_or_vec(ch, fsout);
    } else if(g_QCompState.read_mode == EReadMode_ReadString) {
        handle_read_string(ch, fsout);
    } 
#ifdef ENABLE_SQS_TOKENS
    else if (g_QCompState.read_mode == EReadMode_ReadSQSToken) {
        handle_read_sqs_token(ch, fsout);
    }
#endif
#ifdef ENABLE_ARGPACK
     else if(g_QCompState.read_mode == EReadMode_ReadDollarToken) {
        handle_read_dollar_token(ch, fsout);
    }
#endif
}
void handle_characters(std::string input, FileStream &fsout) {
    std::string::iterator it = input.begin();
    while(it != input.end()) {
        char ch = *it;
        handle_character(ch, fsout);
        it++;
    }

    if(!g_QCompState.current_token.empty()) {
        emit_token(g_QCompState.current_token, fsout);
        g_QCompState.current_token.clear();
    }
}