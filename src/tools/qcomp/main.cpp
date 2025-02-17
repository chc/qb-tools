#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <locale>
#include <codecvt>
#include <stack>
#include <queue>

#include <FileStream.h>
#include <crc32.h>
#include "qcomp.h"

#include <NameToken.h>
#include <IntegerToken.h>
#include <FloatToken.h>
#include <EqualsToken.h>
#include <ChecksumNameToken.h>
#include <EndOfLineToken.h>
#include <EndOfLineNumberToken.h>
#include <PairToken.h>
#include <VectorToken.h>
#include <StartArrayToken.h>
#include <EndArrayToken.h>
#include <StartStructToken.h>
#include <EndStructToken.h>
#include <StringToken.h>
#include <LocalStringToken.h>
#include <ScriptToken.h>
#include <EndScriptToken.h>

#include <OpenParenthesisToken.h>
#include <CloseParenthesisToken.h>
#include <NotToken.h>

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
#include <BreakToken.h>
#include <ReturnToken.h>
#include <AllArgsToken.h>
#include <ColonToken.h>
#include <CommaToken.h>
#include <ArgToken.h>

#include <SwitchToken.h>
#include <CaseToken.h>
#include <DefaultToken.h>
#include <EndSwitchToken.h>

#include <AndToken.h>
#include <OrToken.h>

#include <RandomRangeToken.h>

#include <RandomToken.h>
#include <RandomNoRepeatToken.h>
#include <RandomPermuteToken.h>
#include <JumpToken.h>

#if QTOKEN_SUPPORT_LEVEL > 1
    #define ENABLE_NEWIFS
    #include <FastIfToken.h>
    #include <FastElseToken.h>

    #define ENABLE_SHORTJUMP
    #include <ShortJumpToken.h>
#else
    #include <IfToken.h>
    #include <ElseToken.h>
#endif

#include <EndIfToken.h>

#if QTOKEN_SUPPORT_LEVEL > 2
    #define ENABLE_INLINEPACKSTRUCT
    #define ENABLE_ARGPACK
    #include <InlinePackStructToken.h>
    #include <ArgumentPackToken.h>
#endif

#if QTOKEN_SUPPORT_LEVEL > 3
    #define ENABLE_WIDESTRING
    #include <WideStringToken.h>
#endif

#if QTOKEN_SUPPORT_LEVEL > 4
    #define ENABLE_ELSEIF
    #define ENABLE_NOTEQUAL

    #include <ElseIfToken.h>
    #include <NotEqualToken.h>
#endif

#if QTOKEN_SUPPORT_LEVEL > 5
    #include <StringQSToken.h>
    #define ENABLE_SQS_TOKENS
#endif

#if QTOKEN_SUPPORT_LEVEL > 6
    #include <RandomIntegerToken.h>
    #include <RandomFloatToken.h>
#endif

class RandomData {
public:
    RandomData(RandomData* prev, RandomToken* root) {
        this->prev = prev;
        this->root = root;
        end_token = nullptr;
    }
    RandomToken* root;
    std::stack<QScriptToken*> values;
    std::stack<JumpToken*> jumps;
    QScriptToken* end_token;

    RandomData* prev;
};

enum EReadMode {
    EReadMode_ReadName,
    EReadMode_ReadPairOrVector,
    EReadMode_ReadString,
#ifdef ENABLE_SQS_TOKENS
    EReadMode_ReadSQSToken,
#endif
#ifdef ENABLE_ARGPACK
    EReadMode_ReadDollarToken,
#endif
};

//these state variables are used interchangably depending on the read mode, so the names are a bit generic due to this
typedef struct {
    int read_index;
    EReadMode read_mode;
    std::map<uint32_t, std::string> checksum_names;
    std::string current_token;
    std::string temp_token;
    int emit_type;
    int last_emitted_type;

    bool use_eol_line_numbers;
    int current_line_number;

    std::stack<QScriptToken*> if_token_list; //used for offset writing after
#ifdef ENABLE_INLINEPACKSTRUCT
    bool do_inlinestruct_token;
#endif
    bool got_negate;

#ifdef ENABLE_ARGPACK
    bool do_arg_pack;
#ifdef WITH_SYMBOL_SUPPORT
        ESymbolType argpack_type;
#endif
    bool argpack_isreqparam;
#endif

#ifdef ENABLE_WIDESTRING
    bool do_widestring;
#endif

    int case_count;
#ifdef ENABLE_SHORTJUMP
    std::stack<QScriptToken *> switch_token_list;
#endif

    std::stack<RandomData*> randoms;
    RandomData* current_random;
    bool next_is_random_start_token;
    bool skip_next_random_jump;
    bool got_empty_random;
    int random_depth;
    int mark_end_of_random;
    bool random_read_weight;
    int random_next_weight;
} QCompState;

QCompState g_QCompState;
FILE* mp_input_fd;

void handle_characters(std::string input, FileStream &fsout);
void handle_character(char ch, FileStream &fsout);
bool handle_keyword_check(std::string token, FileStream &fs_out);
void update_if_offsets(FileStream &fs_out);
void update_switch_offsets(FileStream& fs_out);
void update_random_offsets(FileStream& fs_out);
void emit_token(int type, FileStream &fs_out);
void emit_pair_or_vec(std::string token, FileStream &fs_out);
#ifdef ENABLE_SQS_TOKENS
void emit_sqs_token(std::string token, FileStream &fs_out);
#endif

void handle_read_dollar_token(char ch, FileStream &fs_out);
void handle_dollar_char_str(std::string &accum);
bool handle_dollar_char_accum(char ch, std::string &accum);
void append_to_random_or_delete(QScriptToken* token);
int calculate_random_token_num_items();

uint32_t gen_checksum(std::string str, bool with_conversion) {
    if(with_conversion && str.length() > 2 && str.length() <= 10 && str.compare(0,2,"0x") == 0) {
        uint32_t v = (uint32_t)strtoul(str.c_str(), NULL, 16);
        return v;
    }


    uint32_t checksum = thps_gen_checksum(str.c_str());

    g_QCompState.checksum_names[checksum] = str;
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
    bool got_digit = false;
    std::string::iterator it = s.begin();
    while(it != s.end()) {
        char ch = *it;
        bool is_digit = isdigit(ch);
        if(is_digit) {
            got_digit = true;
        }
        if(!is_digit && ch != '.' && ch != '-') {
            return false;
        }
        it++;
    }
    
    return got_digit;
}
void emit_name(std::string name, FileStream &fs_out) {
    if(handle_keyword_check(name, fs_out)) {
        return;
    }
    assert(!name.empty());
    uint32_t checksum = gen_checksum(name, false);

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

        if (g_QCompState.random_read_weight) {
            g_QCompState.random_read_weight = false;
            g_QCompState.random_next_weight = v;
            return;
        }

        IntegerToken *it = new IntegerToken(v);

        it->Write(&fs_out);

        g_QCompState.last_emitted_type = it->GetType();

        append_to_random_or_delete(it);
    } else if(string_is_float(current_token)) {
        float f = atof(current_token.c_str());
        if(g_QCompState.got_negate) {
            g_QCompState.got_negate = false;
            f = -f;
        }
        FloatToken *ft = new FloatToken(f);
        ft->Write(&fs_out);

        g_QCompState.last_emitted_type = ft->GetType();

        append_to_random_or_delete(ft);
    } else {
        std::string::iterator it = current_token.begin();

        std::string accum;
        bool got_less = false;
        int dot_count = 0;
#ifdef ENABLE_ARGPACK
        bool got_dollar = false;
        std::string dollar_accum;
#endif

        //ideally this logic gets cleaned up, would be best to eliminate it and have it exist solely within one of the existing functions
        while(it != current_token.end()) {
            char ch = *it;
            uint8_t token = 0;

#ifdef ENABLE_ARGPACK
            if(got_dollar) {
                if(!handle_dollar_char_accum(ch, dollar_accum)) {
                    if(!dollar_accum.empty()) {
                        handle_dollar_char_str(dollar_accum);
                    } else {
                        assert(false);
                    }
                    got_dollar = false;
                }

                it++;
                continue;
            }
#endif
            switch(ch) {
                case '.':
                    if(!got_less) {
                        token = ESCRIPTTOKEN_DOT;
                    } else {
                        dot_count++;
                    }                    
                    break;
                case '&':
                    token = ESCRIPTTOKEN_ARG;
                break;
                case '<':
                    got_less = true;
                break;
                case '>':
                    if(got_less && dot_count == 3) {
                        got_less = false;
                        dot_count = 0;
                        token = ESCRIPTTOKEN_KEYWORD_ALLARGS;
                    } else {
                        assert(false);
                    }
                break;
#ifdef ENABLE_ARGPACK
                case '$':
                    got_dollar = true;                   
                break;                
#endif
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
        if(!accum.empty()) {
            emit_name(accum, fs_out);
        }
        
    }    
}
void emit_token(int type, FileStream &fs_out) {
    if (RandomToken::is_random_token((EScriptToken)g_QCompState.last_emitted_type)) {
        if (type == ESCRIPTTOKEN_OPENPARENTH) { //do not emit open parenthesis token if last token was random token
            return;
        }
    }
    if (type == ESCRIPTTOKEN_CLOSEPARENTH && g_QCompState.random_depth > 0) {
        g_QCompState.random_depth--;
        g_QCompState.mark_end_of_random++;
        return;
    }

   QScriptToken *token = nullptr;

   bool no_free = false;

   switch(type) {
        case ESCRIPTTOKEN_EQUALS:
            token = new EqualsToken;
        break;
        case ESCRIPTTOKEN_ENDOFLINE:
            token = new EndOfLineToken;
            g_QCompState.current_line_number++;
        break;
        case ESCRIPTTOKEN_ENDOFLINENUMBER:
            token = new EndOfLineNumberToken(g_QCompState.current_line_number++);
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
        case ESCRIPTTOKEN_KEYWORD_SWITCH:
            token = new SwitchToken;
        break;
        case ESCRIPTTOKEN_KEYWORD_CASE:
            token = new CaseToken;
        break;
        case ESCRIPTTOKEN_KEYWORD_DEFAULT:
            token = new DefaultToken;
        break;
        case ESCRIPTTOKEN_KEYWORD_ENDSWITCH:
            token = new EndSwitchToken;
        break;
#ifdef ENABLE_SHORTJUMP
        case ESCRIPTTOKEN_SHORTJUMP:
            assert(false);
        break;
#endif
#ifdef ENABLE_INLINEPACKSTRUCT
        case ESCRIPTTOKEN_INLINEPACKSTRUCT:
        #ifdef WITH_SYMBOL_SUPPORT
            token = new InlinePackStructToken;
        #else
            assert(false);
        #endif
        break;
#endif
#ifndef ENABLE_NEWIFS
        case ESCRIPTTOKEN_KEYWORD_IF:
            token = new IfToken;
        break;
        case ESCRIPTTOKEN_KEYWORD_ELSE:
            token = new ElseToken;
        break;
#endif
        case ESCRIPTTOKEN_KEYWORD_ENDIF:
            token = new EndIfToken;
            g_QCompState.if_token_list.push(token);
            no_free = true;
        break;
#ifdef ENABLE_NEWIFS
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
#endif
#ifdef ENABLE_ELSEIF
        case ESCRIPTTOKEN_KEYWORD_ELSEIF:
            token = new ElseIfToken;
            g_QCompState.if_token_list.push(token);
            no_free = true;
        break;
#endif
        case ESCRIPTTOKEN_KEYWORD_RANDOM_PERMUTE:
            if (token == nullptr)
                token = new RandomPermuteToken(calculate_random_token_num_items());
        case ESCRIPTTOKEN_KEYWORD_RANDOM_NO_REPEAT:
            if (token == nullptr)
                token = new RandomNoRepeatToken(calculate_random_token_num_items());
        case ESCRIPTTOKEN_KEYWORD_RANDOM:
            if(token == nullptr)
                token = new RandomToken(calculate_random_token_num_items());
            
            
            g_QCompState.current_random = new RandomData(g_QCompState.current_random, reinterpret_cast<RandomToken*>(token));
            if (g_QCompState.current_random->prev != nullptr) {
                g_QCompState.current_random->prev->values.push(token);
                g_QCompState.current_random->prev->root->SetWeight(g_QCompState.current_random->prev->values.size() - 1, g_QCompState.random_next_weight);
                g_QCompState.random_next_weight = 1;
            }
            g_QCompState.randoms.push(g_QCompState.current_random);

            g_QCompState.skip_next_random_jump = true;
            g_QCompState.random_depth++;
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
#ifdef ENABLE_NOTEQUAL
        case ESCRIPTTOKEN_NOTEQUAL:
            token = new NotEqualToken;
        break;
#endif
        case ESCRIPTTOKEN_COLON:
            token = new ColonToken;
        break;
        case ESCRIPTTOKEN_ARG:
            token = new ArgToken;
        break;
        case ESCRIPTTOKEN_COMMA:
            token = new CommaToken;
        break;
        case ESCRIPTTOKEN_OR:
            token = new OrToken;
        break;
        case ESCRIPTTOKEN_AND:
            token = new AndToken;
        break;
#if QTOKEN_SUPPORT_LEVEL > 6
        case ESCRIPTTOKEN_KEYWORD_RANDOMINTEGER:
            token = new RandomIntegerToken;
        break;
        case ESCRIPTTOKEN_KEYWORD_RANDOMFLOAT:
            token = new RandomFloatToken;
        break;
#endif //QTOKEN_SUPPORT_LEVEL > 6
        case ESCRIPTTOKEN_KEYWORD_RANDOM_RANGE:
            token = new RandomRangeToken;
        break;
        case ESCRIPTTOKEN_JUMP:
            token = new JumpToken;
            assert(g_QCompState.random_depth > 0);
            assert(g_QCompState.current_random);
            if (g_QCompState.current_random->jumps.size()+1 >= g_QCompState.current_random->root->GetNumItems()) {
                assert(g_QCompState.current_random->prev);
                g_QCompState.current_random->prev->jumps.push(reinterpret_cast<JumpToken*>(token));
            }
            else {
                g_QCompState.current_random->jumps.push(reinterpret_cast<JumpToken*>(token));
            }

            if (g_QCompState.got_empty_random) {
                g_QCompState.got_empty_random = false;
                assert(g_QCompState.next_is_random_start_token);
                assert(!g_QCompState.mark_end_of_random);
                append_to_random_or_delete(token);
                g_QCompState.next_is_random_start_token = true; //set this back, because it was an empty token ie: Random(@ @ @"hello") (2 emptys and a string)
            }
            
            no_free = true;
        break;
        default:
            assert(false);
   }
#ifdef ENABLE_ARGPACK
   if(type == ESCRIPTTOKEN_ARG) {
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
   }
#endif

#ifdef ENABLE_INLINEPACKSTRUCT
   if(g_QCompState.do_inlinestruct_token) {
    #ifdef WITH_SYMBOL_SUPPORT
        g_QCompState.do_inlinestruct_token = false;
        InlinePackStructToken ipst = InlinePackStructToken();
        ipst.Write(&fs_out);
    #else
        assert(false);
    #endif
   }
#endif

#ifdef ENABLE_SHORTJUMP
   bool insert_shortjump_before = false;
   bool insert_shortjump_after = false;
#endif
   bool is_switch = false;

   switch(type) {
        case ESCRIPTTOKEN_KEYWORD_SWITCH:
            g_QCompState.case_count = 1;
            is_switch = true;
        break;
        case ESCRIPTTOKEN_KEYWORD_ENDSWITCH:
            g_QCompState.case_count = 0;
            is_switch = true;
        break;
        case ESCRIPTTOKEN_KEYWORD_CASE:
        case ESCRIPTTOKEN_KEYWORD_DEFAULT:
            is_switch = true;
            g_QCompState.case_count++;
            #ifdef ENABLE_SHORTJUMP
            if(g_QCompState.case_count > 2) {
                insert_shortjump_before = true;    
            }
            insert_shortjump_after = true;
            #endif
            
   }
#ifdef ENABLE_SHORTJUMP
   if(insert_shortjump_before) {
        ShortJumpToken *sjt = new ShortJumpToken;
        sjt->Write(&fs_out);
        g_QCompState.switch_token_list.push(sjt);
   }
#endif

   g_QCompState.last_emitted_type = token->GetType();

   token->Write(&fs_out);
   if (g_QCompState.mark_end_of_random) {
       while (g_QCompState.mark_end_of_random > 0) {
           g_QCompState.current_random->end_token = token;
           g_QCompState.current_random = g_QCompState.current_random->prev;
           g_QCompState.mark_end_of_random--;
       }

       no_free = true;
   }
#ifdef ENABLE_SHORTJUMP
    if(is_switch) {
        no_free = true;
        g_QCompState.switch_token_list.push(token);
    }

   if(insert_shortjump_after) {
        ShortJumpToken *sjt = new ShortJumpToken;
        sjt->Write(&fs_out);
        g_QCompState.switch_token_list.push(sjt);
   }
#endif

   if (g_QCompState.next_is_random_start_token && token->GetType() != ESCRIPTTOKEN_JUMP) {
       g_QCompState.next_is_random_start_token = false;
       if (!RandomToken::is_random_token(token->GetType())) {
           g_QCompState.current_random->values.push(token);
           g_QCompState.current_random->root->SetWeight(g_QCompState.current_random->values.size() - 1, g_QCompState.random_next_weight);
           g_QCompState.random_next_weight = 1;
       }       
       
       no_free = true;
   }

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
#if QTOKEN_SUPPORT_LEVEL > 6
    else if(token.compare("RandomInt") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_RANDOMINTEGER, fs_out);
    } else if(token.compare("RandomFloat") == 0) {
        emit_token(ESCRIPTTOKEN_KEYWORD_RANDOMFLOAT, fs_out);
    }
#endif //QTOKEN_SUPPORT_LEVEL > 6
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
#ifdef ENABLE_SQS_TOKENS
void emit_sqs_token(std::string token, FileStream &fs_out) {
    std::string t = token.substr(4);
    size_t end = t.find_first_of(')');
    assert(end != std::string::npos);
    t = t.substr(0, end);
    int32_t v = (int32_t)strtol(t.c_str(), NULL, 10);

    StringQSToken sqs(v);
    sqs.Write(&fs_out);
    
}
#endif
void emit_pair_or_vec(std::string token, FileStream &fs_out) {
    QScriptToken* script_token;
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
        
        script_token = new PairToken(v[0], v[1]);
        script_token->Write(&fs_out);


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

        script_token = new VectorToken(v[0], v[1], v[2]);
        script_token->Write(&fs_out);

    }

    append_to_random_or_delete(script_token);
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

int main(int argc, const char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s (options) [in] [out]\n", argv[0]);
        fprintf(stderr, "options: \n");
        fprintf(stderr, "\t-linenumber - Enable line numbers\n");
        return -1;
    }

    g_QCompState.use_eol_line_numbers = false;

    int arg_index = 1;
    for (int i = 1; i < argc - 1; i++) {
        if (strstr(argv[i], "-linenumber")) {
            g_QCompState.use_eol_line_numbers = true;
            arg_index = i+1;
        }
    }

    if (arg_index+1 >= argc) {
        fprintf(stderr, "missing expected file params, run without params to see usage!\n");
        return -1;
    }

    mp_input_fd = fopen(argv[arg_index], "r");

    if(!mp_input_fd) {
        fprintf(stderr, "Failed to open input file: %s\n", argv[arg_index]);
        return -1;
    }

    FileStream fsout(argv[arg_index + 1], true);

    if(!fsout.IsFileOpened()) {
        fprintf(stderr, "Failed to open file: %s\n", argv[arg_index + 1]);
        return -1;
    }

    g_QCompState.read_mode = EReadMode_ReadName;

    g_QCompState.current_line_number = 1;

#ifdef ENABLE_ARGPACK
    g_QCompState.do_arg_pack = false;
    g_QCompState.argpack_isreqparam = false;
#endif
#ifdef ENABLE_INLINEPACKSTRUCT
    g_QCompState.do_inlinestruct_token = false;
#endif
    g_QCompState.got_negate = false;
#ifdef ENABLE_WIDESTRING
    g_QCompState.do_widestring = false;
#endif
    g_QCompState.next_is_random_start_token = false;

    g_QCompState.mark_end_of_random = 0;
    g_QCompState.skip_next_random_jump = false;
    g_QCompState.last_emitted_type = 0;
    g_QCompState.random_depth = 0;
    g_QCompState.random_read_weight = false;
    g_QCompState.random_next_weight = 1;
    g_QCompState.got_empty_random = false;

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

#ifdef ENABLE_NEWIFS
    update_if_offsets(fsout);
#endif
#ifdef ENABLE_SHORTJUMP
    update_switch_offsets(fsout);
#endif
    update_random_offsets(fsout);
    return 0;
}

#ifdef ENABLE_SHORTJUMP
void update_switch_offsets(FileStream &fs_out) {
    EndSwitchToken *last_endswitch_token = NULL;
    QScriptToken *last_case_or_default_token = NULL;
    ShortJumpToken *last_shortjump_token = NULL;

    std::stack<EndSwitchToken *> endswitch_stack;


    while(!g_QCompState.switch_token_list.empty()) {
        QScriptToken *token = g_QCompState.switch_token_list.top();
        g_QCompState.switch_token_list.pop();

        switch(token->GetType()) {
            case ESCRIPTTOKEN_SHORTJUMP:
                if(last_shortjump_token != NULL) {
                    last_shortjump_token->RewriteOffset(&fs_out, last_endswitch_token->GetFileOffset() - last_shortjump_token->GetFileOffset());
                }
                last_shortjump_token = reinterpret_cast<ShortJumpToken*>(token);
            break;
            case ESCRIPTTOKEN_KEYWORD_ENDSWITCH:
                endswitch_stack.push(last_endswitch_token);
                last_endswitch_token = reinterpret_cast<EndSwitchToken*>(token);
            break;
            case ESCRIPTTOKEN_KEYWORD_SWITCH:
                assert(last_endswitch_token);
                assert(!endswitch_stack.empty());
                last_endswitch_token = endswitch_stack.top();
                endswitch_stack.pop();
                last_shortjump_token = NULL;
                last_case_or_default_token = NULL;
            break;
            case ESCRIPTTOKEN_KEYWORD_CASE:
            case ESCRIPTTOKEN_KEYWORD_DEFAULT:
                if(last_case_or_default_token == NULL) {
                    assert(last_endswitch_token);
                    last_shortjump_token->RewriteOffset(&fs_out, last_endswitch_token->GetFileOffset() - last_shortjump_token->GetFileOffset() - sizeof(uint8_t));
                } else {
                    last_shortjump_token->RewriteOffset(&fs_out, last_case_or_default_token->GetFileOffset() - last_shortjump_token->GetFileOffset() - sizeof(uint8_t));
                }
                
                last_case_or_default_token = token;
                last_shortjump_token = NULL;
            break;
        }
    }
    assert(endswitch_stack.empty());
}
#endif

#ifdef ENABLE_NEWIFS
void update_if_offsets(FileStream &fs_out) {
    EndIfToken* last_endif_token = NULL;
    FastElseToken* last_else_token = NULL;
#ifdef ENABLE_ELSEIF
    ElseIfToken* last_elseif_token = NULL;
#endif

    std::stack<FastElseToken *> else_stack;
#ifdef ENABLE_ELSEIF
    std::stack<ElseIfToken *> elseif_stack;
#endif
    std::stack<EndIfToken *> endif_stack;

    while(!g_QCompState.if_token_list.empty()) {
        QScriptToken *token = g_QCompState.if_token_list.top();

        FastIfToken *if_token = NULL;

#ifdef ENABLE_ELSEIF
        ElseIfToken *elseif_token = NULL;
#endif

        switch(token->GetType()) {
            case ESCRIPTTOKEN_KEYWORD_FASTIF:
                 if_token = reinterpret_cast<FastIfToken*>(token);
                //set last elseif, or else offset
                #ifdef ENABLE_ELSEIF
                if(last_elseif_token != NULL) {
                    if_token->RewriteOffset(&fs_out, last_elseif_token->GetFileOffset() - if_token->GetFileOffset() - sizeof(uint8_t));
                } else
                #endif //ENABLE_ELSEIF
                 if(last_else_token != NULL) {
                    if_token->RewriteOffset(&fs_out, last_else_token->GetFileOffset() - if_token->GetFileOffset() + sizeof(uint16_t));
                }  else if(last_endif_token != NULL) {
                    if_token->RewriteOffset(&fs_out, last_endif_token->GetFileOffset() - if_token->GetFileOffset());
                } else {
                    assert(false);
                }

                assert(!endif_stack.empty());
                last_endif_token = endif_stack.top();
                endif_stack.pop();

                assert(!else_stack.empty());
                last_else_token = else_stack.top();
                else_stack.pop();

#ifdef ENABLE_ELSEIF
                assert(!elseif_stack.empty());
                last_elseif_token = elseif_stack.top();
                elseif_stack.pop();
#endif // ENABLE_ELSEIF

            break;
            case ESCRIPTTOKEN_KEYWORD_FASTELSE:
                assert(last_endif_token);
                last_else_token = reinterpret_cast<FastElseToken*>(token);

                //set last endif offset
                last_else_token->RewriteOffset(&fs_out, last_endif_token->GetFileOffset() - last_else_token->GetFileOffset());
            break;
#ifdef ENABLE_ELSEIF
            case ESCRIPTTOKEN_KEYWORD_ELSEIF:
                elseif_token = reinterpret_cast<ElseIfToken*>(token);

                //set last endif, last elseif, or last else offset
                if(last_elseif_token != NULL) {
                    elseif_token->SetNextOffset(&fs_out, last_elseif_token->GetFileOffset() - elseif_token->GetFileOffset() - sizeof(uint8_t));
                } else if(last_else_token != NULL) {
                    elseif_token->SetNextOffset(&fs_out, last_else_token->GetFileOffset() - elseif_token->GetFileOffset()  + sizeof(uint16_t));
                } else if (last_endif_token != NULL) {
                    elseif_token->SetNextOffset(&fs_out, last_endif_token->GetFileOffset() - elseif_token->GetFileOffset());    
                } else {
                    assert(false);
                }
                
                elseif_token->SetEndIfOffset(&fs_out, last_endif_token->GetFileOffset() - elseif_token->GetFileOffset() - sizeof(uint16_t));
                
                last_elseif_token = elseif_token;
            break;
#endif
            case ESCRIPTTOKEN_KEYWORD_ENDIF:
                endif_stack.push(last_endif_token);
                last_endif_token = reinterpret_cast<EndIfToken*>(token);
                
                else_stack.push(last_else_token);
                last_else_token = NULL;
#ifdef ENABLE_ELSEIF
                elseif_stack.push(last_elseif_token);
                last_elseif_token = NULL;
#endif
            break;
        }

        g_QCompState.if_token_list.pop();
    }
    
    assert(else_stack.empty());
#ifdef ENABLE_ELSEIF
    assert(elseif_stack.empty());
#endif
    assert(endif_stack.empty());
}
#endif
void update_random_offsets(FileStream& fs_out) {
    while (!g_QCompState.randoms.empty()) {
        RandomData* rnd = g_QCompState.randoms.top();
        g_QCompState.randoms.pop();

        for (int i = rnd->root->GetNumItems(); i>0; i--) {
            QScriptToken* token = rnd->values.top();
            rnd->values.pop();
            
            uint32_t diff = token->GetFileOffset() - rnd->root->GetFileOffset();
            diff -= rnd->root->CalculateTokenOffset(i-1);
            rnd->root->SetRandomOffset(i-1, diff);
        }
        assert(rnd->values.empty());

        rnd->root->Rewrite(&fs_out);

        while (!rnd->jumps.empty()) {
            JumpToken* jump_token = rnd->jumps.top();
            rnd->jumps.pop();

            uint32_t diff = rnd->end_token->GetFileOffset() - jump_token->GetFileOffset() - sizeof(uint32_t) - sizeof(uint8_t);
            jump_token->RewriteOffset(&fs_out, diff);
        }
    }
}
#ifdef ENABLE_ARGPACK
bool handle_dollar_char_accum(char ch, std::string &accum) {
    switch(ch) {
#ifdef ENABLE_WIDESTRING
        case '\'': //wide string token
            if(g_QCompState.do_widestring) {
                return false;
            }
            g_QCompState.do_widestring = true;
            break;
#endif
        case '$':
            return false;
        case '{':
            accum.clear();
            g_QCompState.do_inlinestruct_token = true;
            g_QCompState.do_arg_pack = false;
            return false;
        default:
            accum += ch;
        break;
    }
    return true;
}
#endif

void handle_dollar_char_str(std::string &accum) {
#ifdef WITH_SYMBOL_SUPPORT
    if (accum.substr(0, 4).compare("req_") == 0) {
        g_QCompState.argpack_isreqparam = true;
        accum = accum.substr(4);
    }
    if(accum.compare("int") == 0) {
        g_QCompState.argpack_type = ESYMBOLTYPE_INTEGER;
    } else if(accum.compare("float") == 0) {
        g_QCompState.argpack_type = ESYMBOLTYPE_FLOAT;
    } else if(accum.compare("string") == 0) {
        g_QCompState.argpack_type = ESYMBOLTYPE_STRING;
    } else if(accum.compare("localstring") == 0) {
        g_QCompState.argpack_type = ESYMBOLTYPE_LOCALSTRING;
    } else if(accum.compare("pair") == 0) {
        g_QCompState.argpack_type = ESYMBOLTYPE_PAIR;
    } else if(accum.compare("vec") == 0) {
        g_QCompState.argpack_type = ESYMBOLTYPE_VECTOR;
    } else if(accum.compare("script") == 0) {
        g_QCompState.argpack_type = ESYMBOLTYPE_QSCRIPT;
    } else if(accum.compare("cfunc") == 0) {
        g_QCompState.argpack_type = ESYMBOLTYPE_CFUNCTION;
    } else if(accum.compare("struct") == 0) {
        g_QCompState.argpack_type = ESYMBOLTYPE_STRUCTURE;
    } else if(accum.compare("ptr_struct") == 0) {
        g_QCompState.argpack_type = ESYMBOLTYPE_STRUCTUREPOINTER;
    } else if(accum.compare("array") == 0) {
        g_QCompState.argpack_type = ESYMBOLTYPE_ARRAY;
    } else if(accum.compare("name") == 0) {
        g_QCompState.argpack_type = ESYMBOLTYPE_NAME;
    } else if(accum.compare("none") == 0) {
        g_QCompState.argpack_type = ESYMBOLTYPE_NONE;
    } else {
        assert(false);
    }
    #else
    assert(false);
    #endif

    accum.clear();
}
#ifdef ENABLE_ARGPACK
void handle_read_dollar_token(char ch, FileStream &fs_out) {
    if(!handle_dollar_char_accum(ch, g_QCompState.temp_token)) {
        g_QCompState.read_mode = EReadMode_ReadName;
        
        #ifdef ENABLE_WIDESTRING
        if(g_QCompState.do_widestring) {
            std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff,
                    std::codecvt_mode::little_endian>, char16_t> cnv;
            std::u16string s = cnv.from_bytes(g_QCompState.temp_token);

            WideStringToken wst(s);
            wst.Write(&fs_out);
            g_QCompState.temp_token.clear();
            g_QCompState.do_widestring = false;
            return;
        }
        #endif
        
        g_QCompState.do_arg_pack = true;

        if(!g_QCompState.temp_token.empty()) {
            handle_dollar_char_str(g_QCompState.temp_token);
        } else {
            if(g_QCompState.do_inlinestruct_token) {
                g_QCompState.do_arg_pack = false;
                emit_token(ESCRIPTTOKEN_STARTSTRUCT, fs_out);
            }            
        }
    }
}
#endif
int calculate_random_token_num_items() {
    long pos = ftell(mp_input_fd);

    int num_items = 0;
    int depth = 1;

    while (depth > 0) {
        int c = fgetc(mp_input_fd);
        if (c == EOF) {
            break;
        }
        switch (c) {
            case '@':
                if(depth == 1)
                    num_items++;
            break;
            case '(':
                depth++;
            break;
            case ')':
                depth--;
                break;
        }
    }


    fseek(mp_input_fd, pos, SEEK_SET);
    return num_items;
}
void append_to_random_or_delete(QScriptToken* token) {
    if (g_QCompState.next_is_random_start_token) {
        g_QCompState.next_is_random_start_token = false;
        g_QCompState.current_random->values.push(token);
        g_QCompState.current_random->root->SetWeight(g_QCompState.current_random->values.size() - 1, g_QCompState.random_next_weight);
        g_QCompState.random_next_weight = 1;

    }
    else if (g_QCompState.mark_end_of_random) {
        g_QCompState.current_random->end_token = token;
        g_QCompState.current_random = g_QCompState.current_random->prev;
        g_QCompState.mark_end_of_random--;
    }
    else {
        delete token;
    }
}