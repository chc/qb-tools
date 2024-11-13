#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <FileStream.h>
#include <crc32.h>
#include <stack>
#include "qcomp.h"

#include <NameToken.h>
#include <IntegerToken.h>
#include <FloatToken.h>
#include <EqualsToken.h>
#include <ChecksumNameToken.h>
#include <EndOfLineToken.h>
#include <PairToken.h>
#include <VectorToken.h>
#include <StartArrayToken.h>
#include <EndArrayToken.h>
#include <StartStructToken.h>
#include <EndStructToken.h>
#include <StringToken.h>
#include <LocalStringToken.h>
#include <WideStringToken.h>
#include <ScriptToken.h>
#include <EndScriptToken.h>
#include <InlinePackStructToken.h>
#include <ArgumentPackToken.h>


#include <FastIfToken.h>
#include <IfToken.h>
#include <FastElseToken.h>
#include <ElseToken.h>
#include <ElseIfToken.h>
#include <EndIfToken.h>

#include <OpenParenthesisToken.h>
#include <CloseParenthesisToken.h>
#include <NotToken.h>
#include <NotEqualToken.h>
#include <AddToken.h>
#include <MinusToken.h>
#include <MultiplyToken.h>
#include <DivideToken.h>
#include <RepeatToken.h>
#include <BeginToken.h>
#include <GreaterThanEqualToken.h>
#include <GreaterThanToken.h>
#include <LessThanEqualToken.h>
#include <LessThanToken.h>
#include <DotToken.h>
#include <StringQSToken.h>
#include <BreakToken.h>
#include <ReturnToken.h>
#include <AllArgsToken.h>
#include <ColonToken.h>
#include <ArgToken.h>

enum EReadMode {
    EReadMode_ReadName,
    EReadMode_ReadPairOrVector,
    EReadMode_ReadString,
    EReadMode_ReadSQSToken,
};

//these state variables are used interchangably depending on the read mode, so the names are a bit generic due to this
typedef struct {
    float floats[3];
    int read_index;
    EReadMode read_mode;
    std::map<uint32_t, std::string> checksum_names;
    std::string current_token;
    std::string temp_token;
    int emit_type;

    bool use_eol_line_numbers;
    bool use_new_ifs;

    std::stack<QScriptToken*> if_token_list; //used for offset writing after
    bool got_dollar_token;
    bool do_inlinestruct_token;
    bool got_negate;
} QCompState;

QCompState g_QCompState;

void handle_characters(std::string input, FileStream &fsout);
void handle_character(char ch, FileStream &fsout);
bool handle_keyword_check(std::string token, FileStream &fs_out);
void update_if_offsets(FileStream &fs_out);
void emit_token(int type, FileStream &fs_out);
void emit_pair_or_vec(std::string token, FileStream &fs_out);
void emit_sqs_token(std::string token, FileStream &fs_out);

uint32_t gen_checksum(std::string str, bool with_conversion) {
    if(with_conversion && str.length() == 10 && str.compare(0,2,"0x") == 0) {
        int32_t v = (int32_t)strtol(str.c_str(), NULL, 16);
        if(v != 0) {
            return v;
        }
    }

    char *name = strdup(str.c_str());
    int len = strlen(name);

    for(int i=0;i<len;i++) {
        name[i] = tolower(name[i]);
    }

    uint32_t checksum = crc32(0, name, len);

    g_QCompState.checksum_names[checksum] = str;
    free(name);
    return checksum;
}

bool string_is_numeric(std::string &s) {
    if (s.empty()) {
        return false;
    }
    std::string::iterator it = s.begin();
    while(it != s.end()) {
        char ch = *it;
        if(!isdigit(ch) && ch != '-') {
            return false;
        }
        it++;
    }
    return true;
}
bool string_is_float(std::string &s) {
    if (s.empty()) {
        return false;
    }
    std::string::iterator it = s.begin();
    while(it != s.end()) {
        char ch = *it;
        if(!isdigit(ch) && ch != '.' && ch != '-') {
            return false;
        }
        it++;
    }
    return true;
}
void emit_name(std::string name, FileStream &fs_out) {
    assert(!name.empty());
    uint32_t checksum = gen_checksum(name, false);

    if(g_QCompState.got_dollar_token) {
        g_QCompState.got_dollar_token = false;
        ArgumentPackToken apt;
        apt.SetRefType(ESYMBOLTYPE_STRUCTURE);
        if(checksum == REQUIRED_PARAM_VALUE) {
            apt.SetIsRequiredParams(true);
        } else {
            apt.SetIsRequiredParams(false);
        }
        
        apt.WriteExtendedParams(&fs_out);
    }


    NameToken nt(checksum);
    nt.Write(&fs_out);
}
void emit_token(std::string &current_token, FileStream &fs_out) {
    if(handle_keyword_check(current_token, fs_out)) {
        return;
    }
    if(string_is_numeric(current_token)) {
        int32_t v = (int32_t)strtol(current_token.c_str(), NULL, 10);
        if(g_QCompState.got_negate) {
            g_QCompState.got_negate = false;
            v = -v;
        }

        IntegerToken it(v);
        it.Write(&fs_out);
    } else if(string_is_float(current_token)) {
        float f = atof(current_token.c_str());
        if(g_QCompState.got_negate) {
            g_QCompState.got_negate = false;
            f = -f;
        }
        FloatToken ft(f);
        ft.Write(&fs_out);
    } else {
        std::string::iterator it = current_token.begin();

        std::string accum;
        while(it != current_token.end()) {
            char ch = *it;
            uint8_t token = 0;
            switch(ch) {
                case '.':
                    token = ESCRIPTTOKEN_DOT;
                    break;
                case '&':
                    token = ESCRIPTTOKEN_ARG;
                break;
                case '$':
                    g_QCompState.got_dollar_token = true;
                break;                
                default:
                    accum += ch;
                break;
            }
            if(token != 0) {
                if(!accum.empty()) {
                    emit_name(accum, fs_out);
                    accum.clear();
                }
                emit_token(token, fs_out);
            }
            it++;        
        }
        emit_name(accum, fs_out);
        
    }    
}
void emit_token(int type, FileStream &fs_out) {
   QScriptToken *token = NULL;

   bool no_free = false;

   switch(type) {
        case ESCRIPTTOKEN_EQUALS:
            token = new EqualsToken;
            break;
        case ESCRIPTTOKEN_ENDOFLINE:
            token = new EndOfLineToken;
        break;
        case ESCRIPTTOKEN_STARTARRAY:
            token = new StartArrayToken;
        break;
        case ESCRIPTTOKEN_ENDARRAY:
            token = new EndArrayToken;
        break;
        case ESCRIPTTOKEN_STARTSTRUCT:
            token = new StartStructToken;
        break;
        case ESCRIPTTOKEN_ENDSTRUCT:
            token = new EndStructToken;
        break;
        case ESCRIPTTOKEN_KEYWORD_SCRIPT:
            token = new ScriptToken;
        break;
        case ESCRIPTTOKEN_KEYWORD_ENDSCRIPT:
            token = new EndScriptToken;
        break;
        case ESCRIPTTOKEN_INLINEPACKSTRUCT:
            token = new InlinePackStructToken;
        break;
        case ESCRIPTTOKEN_KEYWORD_IF:
            token = new IfToken;
        break;
        case ESCRIPTTOKEN_KEYWORD_ELSE:
            token = new ElseToken;
            g_QCompState.if_token_list.push(token);
            no_free = true;
        break;
        case ESCRIPTTOKEN_KEYWORD_ENDIF:
            token = new EndIfToken;
            g_QCompState.if_token_list.push(token);
            no_free = true;
        break;
        case ESCRIPTTOKEN_KEYWORD_FASTIF:
            token = new FastIfToken;
            g_QCompState.if_token_list.push(token);
            no_free = true;
        break;
        case ESCRIPTTOKEN_KEYWORD_FASTELSE:
            token = new FastElseToken;
            g_QCompState.if_token_list.push(token);
            no_free = true;
        break;
        case ESCRIPTTOKEN_KEYWORD_ELSEIF:
            token = new ElseIfToken;
            g_QCompState.if_token_list.push(token);
            no_free = true;
        break;
        case ESCRIPTTOKEN_OPENPARENTH:
            token = new OpenParenthesisToken;
        break;
        case ESCRIPTTOKEN_CLOSEPARENTH:
            token = new CloseParenthesisToken;
        break;
        case ESCRIPTTOKEN_ADD:
            token = new AddToken;
        break;
        case ESCRIPTTOKEN_MINUS:
            token = new MinusToken;
        break;
        case ESCRIPTTOKEN_MULTIPLY:
            token = new MultiplyToken;
        break;
        case ESCRIPTTOKEN_DIVIDE:
            token = new DivideToken;
        break;
        case ESCRIPTTOKEN_KEYWORD_NOT:
            token = new NotToken;
        break;
        case ESCRIPTTOKEN_KEYWORD_BEGIN:
            token = new BeginToken;
        break;
        case ESCRIPTTOKEN_KEYWORD_REPEAT:
            token = new RepeatToken;
        break;
        case ESCRIPTTOKEN_LESSTHAN:
            token = new LessThanToken;
        break;
        case ESCRIPTTOKEN_LESSTHANEQUAL:
            token = new LessThanEqualToken;
        break;
        case ESCRIPTTOKEN_GREATERTHAN:
            token = new GreaterThanToken;
        break;
        case ESCRIPTTOKEN_GREATERTHANEQUAL:
            token = new GreaterThanEqualToken;
        break;
        case ESCRIPTTOKEN_DOT:
            token = new DotToken;
        break;
        case ESCRIPTTOKEN_KEYWORD_RETURN:
            token = new ReturnToken;
        break;
        case ESCRIPTTOKEN_KEYWORD_ALLARGS:
            token = new AllArgsToken;
        break;
        case ESCRIPTTOKEN_KEYWORD_BREAK:
            token = new BreakToken;
        break;
        case ESCRIPTTOKEN_NOTEQUAL:
            token = new NotEqualToken;
        break;
        case ESCRIPTTOKEN_COLON:
            token = new ColonToken;
        break;
        case ESCRIPTTOKEN_ARG:
            token = new ArgToken;
        break;
        default:
            assert(false);
   }

   if(type == ESCRIPTTOKEN_ARG) {
        if(g_QCompState.got_dollar_token) {
            g_QCompState.got_dollar_token = false;
            ArgumentPackToken apt;
            apt.SetRefType(ESYMBOLTYPE_STRUCTURE);
            apt.SetIsRequiredParams(false);            
            apt.WriteExtendedParams(&fs_out);
        }
   }

   if(g_QCompState.do_inlinestruct_token) {
        g_QCompState.do_inlinestruct_token = false;
        InlinePackStructToken ipst = InlinePackStructToken();
        ipst.Write(&fs_out);
   }

   token->Write(&fs_out);
   if(!no_free) {
        delete token;
   }    
}
bool handle_keyword_check(std::string token, FileStream &fs_out) {
    if (token.compare("script") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_SCRIPT, fs_out);
    } else if (token.compare("endscript") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_ENDSCRIPT, fs_out);        
    } else if(token.compare("if") == 0) {
        if(g_QCompState.use_new_ifs) {
            emit_token(ESCRIPTTOKEN_KEYWORD_FASTIF, fs_out);
        } else {
            emit_token(ESCRIPTTOKEN_KEYWORD_IF, fs_out);
        }
        
    } else if(token.compare("else") == 0) {
        if(g_QCompState.use_new_ifs) {
            emit_token(ESCRIPTTOKEN_KEYWORD_FASTELSE, fs_out);
        } else {
            emit_token(ESCRIPTTOKEN_KEYWORD_ELSE, fs_out);
        }        
    } else if(token.compare("elseif") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_ELSEIF, fs_out);
    } else if(token.compare("endif") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_ENDIF, fs_out);       
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
    } else if(token.compare("!=") == 0) {
        emit_token(ESCRIPTTOKEN_NOTEQUAL, fs_out);
    } else if(token.compare(0, 3, "SQS") == 0 && g_QCompState.emit_type == ESCRIPTTOKEN_OPENPARENTH) {
        g_QCompState.read_mode = EReadMode_ReadSQSToken;
        assert(g_QCompState.emit_type == ESCRIPTTOKEN_OPENPARENTH);
        g_QCompState.emit_type = 0;
        g_QCompState.temp_token = token + "("; //left over ( is still processed later, will need to clear before changing states again too
    } else if(token.compare("return") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_RETURN, fs_out);
    } else if(token.compare("break") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_BREAK, fs_out);
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
        case '$':
            g_QCompState.got_dollar_token = true;
            skip_token = true;
            return;
        break;
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
            g_QCompState.emit_type = ESCRIPTTOKEN_MULTIPLY;
        break;
        case '/':
            g_QCompState.emit_type = ESCRIPTTOKEN_DIVIDE;
        break;
        case ':':
            g_QCompState.emit_type = ESCRIPTTOKEN_COLON;
        break;
        // case '.':
        //     g_QCompState.emit_type = ESCRIPTTOKEN_DOT;
        // break;
        case '{':
            if(g_QCompState.got_dollar_token) {
                g_QCompState.got_dollar_token = false;
                g_QCompState.do_inlinestruct_token = true;
            }
            g_QCompState.emit_type = ESCRIPTTOKEN_STARTSTRUCT;
        break;
        case '}':
            g_QCompState.emit_type = ESCRIPTTOKEN_ENDSTRUCT;
        break;
        case '+':
            g_QCompState.emit_type = ESCRIPTTOKEN_ADD;
        break;
        case '-':
            g_QCompState.got_negate = true;
            return;
        break;
        case ' ':
            if(handle_keyword_check(g_QCompState.current_token, fs_out)) {
                g_QCompState.current_token.clear();
            }
            if(g_QCompState.got_negate) {
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
            g_QCompState.emit_type = ESCRIPTTOKEN_ENDOFLINE;
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
void emit_sqs_token(std::string token, FileStream &fs_out) {
    std::string t = token.substr(4);
    size_t end = t.find_first_of(')');
    assert(end != std::string::npos);
    t = t.substr(0, end);
    int32_t v = (int32_t)strtol(t.c_str(), NULL, 10);

    StringQSToken sqs(v);
    sqs.Write(&fs_out);
    
}
void emit_pair_or_vec(std::string token, FileStream &fs_out) {
    if(token.compare(0, 5, "Pair(") == 0) {
        float v[2];
        std::string t = token.substr(5);
        size_t comma = t.find_first_of(',');
        assert(comma != std::string::npos);

        std::string first = t.substr(0, comma);
        v[0] = atof(first.c_str());

        size_t end = t.find_first_of(')');
        assert(end != std::string::npos);
        std::string second = t.substr(comma+1, end-2);
        v[1] = atof(second.c_str());
        
        PairToken pt(v[0], v[1]);
        pt.Write(&fs_out);


    } else if (token.compare(0, 4, "Vec(") == 0) {
        float v[3];
        std::string t = token.substr(4);
        size_t comma = t.find_first_of(',');
        assert(comma != std::string::npos);
       
        std::string first = t.substr(0, comma);
        v[0] = atof(first.c_str());

        t = t.substr(comma+1);
        comma = t.find_first_of(',');
        assert(comma != std::string::npos);

        std::string second = t.substr(0, comma);
        v[1] = atof(second.c_str());

        t = t.substr(comma+1);
        comma = t.find_first_of(')');
        assert(comma != std::string::npos);

        t = t.substr(0,comma);
        v[2] = atof(t.c_str());

        VectorToken vt(v[0], v[1], v[2]);
        vt.Write(&fs_out);

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
            StringToken st(g_QCompState.current_token);
            st.Write(&fs_out);
        } else if(g_QCompState.emit_type == ESCRIPTTOKEN_LOCALSTRING) {
            LocalStringToken lst(g_QCompState.current_token);
            lst.Write(&fs_out);  
        } else if (g_QCompState.emit_type == ESCRIPTTOKEN_NAME) {
            uint32_t checksum = gen_checksum(g_QCompState.current_token, true);
            if(g_QCompState.got_dollar_token) {
                g_QCompState.got_dollar_token = false;
                ArgumentPackToken apt;
                apt.SetRefType(ESYMBOLTYPE_STRUCTURE);
                if(checksum == REQUIRED_PARAM_VALUE) {
                    apt.SetIsRequiredParams(true);
                } else {
                    apt.SetIsRequiredParams(false);
                }
                
                apt.WriteExtendedParams(&fs_out);
            }
            NameToken nt(checksum);
            nt.Write(&fs_out);
        } else {
            assert(false);
        }
        g_QCompState.read_mode = EReadMode_ReadName;
        g_QCompState.emit_type = 0;
        g_QCompState.current_token.clear();
    }
}
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
void handle_character(char ch, FileStream &fsout) {
    if(g_QCompState.read_mode == EReadMode_ReadName) {
        handle_read_name(ch, fsout);
    } else if(g_QCompState.read_mode == EReadMode_ReadPairOrVector) {
        handle_read_pair_or_vec(ch, fsout);
    } else if(g_QCompState.read_mode == EReadMode_ReadString) {
        handle_read_string(ch, fsout);
    } else if (g_QCompState.read_mode == EReadMode_ReadSQSToken) {
        handle_read_sqs_token(ch, fsout);
    }
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

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s [in] [out]\n", argv[0]);
        return -1;
    }

    FILE *mp_input_fd = fopen(argv[1], "r");

    if(!mp_input_fd) {
        fprintf(stderr, "Failed to open input file: %s\n", argv[1]);
        return -1;
    }

    FileStream fsout(argv[2], true);

    if(!fsout.IsFileOpened()) {
        fprintf(stderr, "Failed to open file: %s\n", argv[2]);
        return -1;
    }

    g_QCompState.read_mode = EReadMode_ReadName;
    g_QCompState.use_eol_line_numbers = false;
    g_QCompState.use_new_ifs = true;
    g_QCompState.got_dollar_token = false;
    g_QCompState.do_inlinestruct_token = false;
    g_QCompState.got_negate = false;
    
    while(true) {
        int ch = fgetc(mp_input_fd);
        if(ch == EOF) {
            break;
        }
       handle_character(ch, fsout);
    }

    if(!g_QCompState.current_token.empty()) {
        emit_token(g_QCompState.current_token, fsout);
    } 

    std::map<uint32_t, std::string>::iterator it = g_QCompState.checksum_names.begin();
    while(it != g_QCompState.checksum_names.end()) {
        std::pair<uint32_t, std::string> p = *it;
        it++;
        if(p.first == 0 || p.second.empty())
            continue;
        ChecksumNameToken token(p.first, p.second.c_str());
        token.Write(&fsout);
    }
    fsout.WriteByte(ESCRIPTTOKEN_ENDOFFILE);

    update_if_offsets(fsout);
    return 0;
}

void update_if_offsets(FileStream &fs_out) {
    EndIfToken* last_endif_token = NULL;
    FastElseToken* last_else_token = NULL;
    ElseIfToken* last_elseif_token = NULL;

    std::stack<EndIfToken *> endif_stack;

    while(!g_QCompState.if_token_list.empty()) {
        QScriptToken *token = g_QCompState.if_token_list.top();


        FastIfToken *if_token = NULL;
        ElseIfToken *elseif_token = NULL;

        switch(token->GetType()) {
            case ESCRIPTTOKEN_KEYWORD_FASTIF:
                if(!endif_stack.empty()) {
                    last_endif_token = endif_stack.top();
                    endif_stack.pop();
                } else {
                    last_endif_token = NULL;
                }
                 if_token = reinterpret_cast<FastIfToken*>(token);
                //set last elseif, or else offset
                if(last_elseif_token != NULL) {
                    if_token->RewriteOffset(&fs_out, last_elseif_token->GetFileOffset() - if_token->GetFileOffset() - sizeof(uint8_t));
                } else if(last_else_token != NULL) {
                    if_token->RewriteOffset(&fs_out, last_else_token->GetFileOffset() - if_token->GetFileOffset() + sizeof(uint16_t));
                }  else if(last_endif_token != NULL) {
                    assert(last_endif_token);
                    if_token->RewriteOffset(&fs_out, last_endif_token->GetFileOffset() - if_token->GetFileOffset());
                } else {
                    assert(false);
                }

                last_else_token = NULL;
                last_elseif_token = NULL;
            break;
            case ESCRIPTTOKEN_KEYWORD_FASTELSE:
                last_else_token = reinterpret_cast<FastElseToken*>(token);
                //set last endif
                last_else_token->RewriteOffset(&fs_out, last_endif_token->GetFileOffset() - last_else_token->GetFileOffset());
            break;
            case ESCRIPTTOKEN_KEYWORD_ELSEIF:
                elseif_token = reinterpret_cast<ElseIfToken*>(token);
                //set last endif, last elseif, or last else

                if(last_elseif_token != NULL) {
                    elseif_token->SetNextOffset(&fs_out, last_elseif_token->GetFileOffset() - elseif_token->GetFileOffset() - sizeof(uint8_t));
                } else if(last_else_token != NULL) {
                    elseif_token->SetNextOffset(&fs_out, last_else_token->GetFileOffset() - elseif_token->GetFileOffset()  + sizeof(uint16_t));
                } else if (last_endif_token != NULL) {
                    elseif_token->SetNextOffset(&fs_out, last_endif_token->GetFileOffset() - elseif_token->GetFileOffset());    
                }
                
                elseif_token->SetEndIfOffset(&fs_out, last_endif_token->GetFileOffset() - elseif_token->GetFileOffset() - sizeof(uint16_t));
                last_elseif_token = elseif_token;
            break;
            case ESCRIPTTOKEN_KEYWORD_ENDIF:
                last_endif_token = reinterpret_cast<EndIfToken*>(token);
                endif_stack.push(last_endif_token);
            break;
        }

        g_QCompState.if_token_list.pop();
    }
    assert(endif_stack.empty());
}