#include "qcomp.h"

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
#if QTOKEN_SUPPORT_LEVEL > 5
        case ESCRIPTTOKEN_KEYWORD_RANDOMINTEGER:
            token = new RandomIntegerToken;
        break;
        case ESCRIPTTOKEN_KEYWORD_RANDOMFLOAT:
            token = new RandomFloatToken;
        break;
#endif //QTOKEN_SUPPORT_LEVEL > 6
#if QTOKEN_SUPPORT_LEVEL > 6
        case ESCRIPTTOKEN_TYPEHINT_CONTROLLER:
            token = new TypeHint_Controller;
        break;
        case ESCRIPTTOKEN_TYPEHINT_ID:
            token = new TypeHint_ID;
        break;
        case ESCRIPTTOKEN_TYPEHINT_USERINPUT:
            token = new TypeHint_UserInput;
        break;
        case ESCRIPTTOKEN_TYPEHINT_CALLBACK:
            token = new TypeHint_Callback;
        break;
        case ESCRIPTTOKEN_TYPEHINT_PLAYLIST:
            token = new TypeHint_Playlist;
        break;
#endif
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