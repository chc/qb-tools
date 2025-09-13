#include "qcomp.h"

QCompState g_QCompState;
FILE* mp_input_fd;

uint32_t gen_checksum(std::string str, bool with_conversion) {
    if(with_conversion && str.length() > 2 && str.length() <= 10 && str.compare(0,2,"0x") == 0) {
        uint32_t v = (uint32_t)strtoul(str.c_str(), NULL, 16);
        return v;
    }

    uint32_t checksum = thps_gen_checksum(str.c_str());

    g_QCompState.checksum_names[checksum] = str;
    return checksum;
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
    } else if(accum.compare("memberfunc") == 0) {
        g_QCompState.argpack_type = ESYMBOLTYPE_MEMBERFUNCTION;
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