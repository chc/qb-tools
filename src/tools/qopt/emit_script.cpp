
#include <cassert>
#include <MemoryStream.h>

#include "qopt.h"
#include <QSymbolToken.h>
#include <QScriptToken.h>
#include <QScriptSymbol.h>

#include <NameToken.h>
#include <NameSymbol.h>
#include <IntegerToken.h>
#include <IntegerSymbol.h>
#include <FloatToken.h>
#include <FloatSymbol.h>

#include <ArraySymbol.h>

#include <FastIfToken.h>
#include <FastElseToken.h>
#include <ElseIfToken.h>
#include <StructureSymbol.h>
#include <InlinePackStructToken.h>
#include <JumpToken.h>
#include <ShortJumpToken.h>

#include <map>
std::map<QScriptToken *, size_t> original_offsets;
std::map<QScriptToken *, size_t> updated_offsets;

extern "C" {
    extern int32_t g_last_script_keyword;
    extern int32_t g_last_script_keyword_write;
}
EScriptToken next_usable_type(std::vector<QScriptToken *>::iterator begin, std::vector<QScriptToken *>::iterator end);

QScriptToken *find_by_offset(std::map<QScriptToken *, size_t> &offsets, size_t offset) {
    std::map<QScriptToken *, size_t>::iterator it = offsets.begin();
    while(it != offsets.end()) {
        if(it->second == offset) {
            return it->first;
        }
        it++;
    }
    return nullptr;
}



bool is_random_token(EScriptToken type) {
    switch(type) {
        case ESCRIPTTOKEN_KEYWORD_RANDOM:
        //case ESCRIPTTOKEN_KEYWORD_RANDOM2:
        case ESCRIPTTOKEN_KEYWORD_RANDOM_PERMUTE:
        //case ESCRIPTTOKEN_KEYWORD_RANDOM_RANGE2:
        //case ESCRIPTTOKEN_KEYWORD_RANDOM_RANGE:
            return true;
        break;
    }
    return false;
}
void rewrite_offsets(IStream *stream, QScriptToken *token) {
    switch(token->GetType()) {
        //case ESCRIPTTOKEN_KEYWORD_FASTELSE:
        case ESCRIPTTOKEN_JUMP:
        //case ESCRIPTTOKEN_SHORTJUMP:
        //case ESCRIPTTOKEN_KEYWORD_ELSEIF:
        assert(false);
    }
    if(is_random_token(token->GetType())) {
        assert(false);
    }
    if(token->GetType() == ESCRIPTTOKEN_KEYWORD_FASTIF) {
        size_t original_offset = original_offsets[token];
        FastIfToken* fif = reinterpret_cast<FastIfToken*>(token);
        size_t endif_offset = fif->GetOffset() + original_offset + sizeof(uint8_t);
        QScriptToken *t = find_by_offset(original_offsets, endif_offset);
        assert(t);

        size_t updated_offset = updated_offsets[t] - fif->GetFileOffset() - sizeof(uint8_t);
        fif->RewriteOffset(stream, updated_offset);
    } else if(token->GetType() == ESCRIPTTOKEN_KEYWORD_FASTELSE) {
        size_t original_offset = original_offsets[token];
        FastElseToken* fif = reinterpret_cast<FastElseToken*>(token);
        size_t endif_offset = fif->GetOffset() + original_offset;
        QScriptToken *t = find_by_offset(original_offsets, endif_offset);
        assert(t);

        size_t updated_offset = updated_offsets[t] - fif->GetFileOffset() - sizeof(uint8_t);
        fif->RewriteOffset(stream, updated_offset);
    } else if(token->GetType() == ESCRIPTTOKEN_KEYWORD_ELSEIF) {
        size_t original_offset = original_offsets[token];
        ElseIfToken* fif = reinterpret_cast<ElseIfToken*>(token);

        //"next" recalculation
        size_t offset = fif->GetNextOffset() + original_offset + sizeof(uint8_t);
        QScriptToken *t = find_by_offset(original_offsets, offset);
        assert(t);

        size_t updated_offset = updated_offsets[t] - fif->GetFileOffset() - sizeof(uint8_t);  
        fif->SetNextOffset(stream, updated_offset);

        //endif recalculation
        offset = fif->GetEndIfOffset() + original_offset + sizeof(uint8_t);
        t = find_by_offset(original_offsets, offset);
        assert(t);

        updated_offset = updated_offsets[t] - fif->GetFileOffset() - sizeof(uint8_t);  
        fif->SetEndIfOffset(stream, updated_offset);
    } else if(token->GetType() == ESCRIPTTOKEN_SHORTJUMP) {
        size_t original_offset = original_offsets[token];
        ShortJumpToken* jt = reinterpret_cast<ShortJumpToken*>(token);
        size_t endif_offset = jt->GetOffset() + original_offset + sizeof(uint8_t);
        QScriptToken *t = find_by_offset(original_offsets, endif_offset);
        assert(t);

        size_t updated_offset = updated_offsets[t] - jt->GetFileOffset() - sizeof(uint16_t);
        jt->RewriteOffset(stream, updated_offset);
    }
}


std::vector<QScriptToken *>::iterator handle_struct_array(uint32_t name, std::vector<QSymbolToken *> &struct_symbols, std::vector<QScriptToken *>::iterator begin, std::vector<QScriptToken *>::iterator end);
std::vector<QScriptToken *>::iterator ReadArray(uint32_t name, std::vector<QScriptToken *>::iterator begin, std::vector<QScriptToken *>::iterator end, ArraySymbol** out) {
    std::vector<QSymbolToken *> syms;
    std::vector<QScriptToken *>::iterator it = handle_struct_array(name, syms, begin, end);
    if(!syms.empty()) {
        assert(syms.size() == 1);

        
        *out = (ArraySymbol*)syms.front();
    } else {
        *out = new ArraySymbol(nullptr, 0);
    }

    return it;

}
std::vector<QScriptToken *>::iterator ReadStructure(std::vector<QScriptToken *>::iterator begin, std::vector<QScriptToken *>::iterator end, StructureSymbol** out) {
    std::vector<QScriptToken *>::iterator it = begin;

    assert((*it)->GetType() == ESCRIPTTOKEN_STARTSTRUCT);
    it++;


    bool in_argument_pack = false;
    bool in_name_mode = true;
    uint32_t name_checksum = 0;

    QSymbolToken *sym = nullptr;

    StructureSymbol *result;
    ArraySymbol *arr_result;

    std::vector<QSymbolToken *> children;

    int depth = 1;
    while(it != end && depth > 0) {
        QScriptToken *t = *it;
        switch(t->GetType()) {
            case ESCRIPTTOKEN_EQUALS: ///XXX: check this
                in_name_mode = false;
            break;
            case ESCRIPTTOKEN_ENDOFLINE:
            case ESCRIPTTOKEN_ENDOFLINENUMBER:
            break;
            case ESCRIPTTOKEN_INLINEPACKSTRUCT:
                assert(false);
            break;
            case ESCRIPTTOKEN_STARTSTRUCT:
                it = ReadStructure(it, end, &result);
                //result->SetIsStructItem(true);
                result->SetNameChecksum(name_checksum);
                children.push_back(result);
                in_name_mode = true;
                continue;
            case ESCRIPTTOKEN_ENDSTRUCT:
                depth--;
                assert(depth==0);
                break;
            case ESCRIPTTOKEN_STARTARRAY:
                it = ReadArray(name_checksum, it+1, end, &arr_result);
                arr_result->SetNameChecksum(name_checksum);
                children.push_back(arr_result);
                in_name_mode = true;
                continue;
            case ESCRIPTTOKEN_ENDARRAY:
                assert(false);
            case ESCRIPTTOKEN_ARGUMENTPACK:
                in_argument_pack = true;
            break;
            case ESCRIPTTOKEN_NAME:
                if(in_name_mode && next_usable_type(it+1, g_Deopt.script_tokens.end()) != ESCRIPTTOKEN_EQUALS) {
                    in_name_mode = false;
                    name_checksum = 0;
                }
                if(in_name_mode) {
                    in_name_mode = false;
                    name_checksum = reinterpret_cast<NameToken*>(t)->GetChecksum();                    
                    break;
                }
            default:
                sym = ConvertToken(t, in_argument_pack);
                assert(sym);
                in_argument_pack = false;
                in_name_mode = true;
                sym->SetNameChecksum(name_checksum);
                sym->SetIsStructItem(true);
                children.push_back(sym);
            break;
        }
        it++;
    }
    
    *out = new StructureSymbol(children);
    return it;
}

void rewrite_inline_structs() {
    std::vector<QScriptToken *>::iterator it = g_Deopt.script_tokens.begin();

    while(it != g_Deopt.script_tokens.end()) {
        QScriptToken *token = *it;        
        if(token->GetType() == ESCRIPTTOKEN_INLINEPACKSTRUCT) {
            InlinePackStructToken* ip = reinterpret_cast<InlinePackStructToken*>(token);
            if(ip->GetValue() == nullptr) {
                StructureSymbol *sym;
                std::vector<QScriptToken *>::iterator it2 = ReadStructure(it+1, g_Deopt.script_tokens.end(), &sym);

                it = g_Deopt.script_tokens.erase(it+1, it2);
                ip->SetValue(sym);
                continue;
            }
        }
        it++;
    }
}

void emit_script() {
    const int TOKEN_BUFF_SIZE = 4096;
    uint8_t *token_buffer = new uint8_t[TOKEN_BUFF_SIZE];

    MemoryStream ms(token_buffer, TOKEN_BUFF_SIZE);
    rewrite_inline_structs();
    std::vector<QScriptToken *>::iterator it = g_Deopt.script_tokens.begin();
    while(it != g_Deopt.script_tokens.end()) {
        QScriptToken *token = *it;
        original_offsets[token] = token->GetFileOffset();
        if(token->GetType() == ESCRIPTTOKEN_INLINEPACKSTRUCT) {
            InlinePackStructToken* ip = reinterpret_cast<InlinePackStructToken*>(token);
            size_t inline_offset = ms.GetOffset() + 7;
   
            int padding = (sizeof(uint32_t)) - (((inline_offset - 1) % sizeof(uint32_t)) + 1);
            assert(padding <= 4);
            ip->SetPadding(padding);
        }
        token->Write(&ms);
        updated_offsets[token] = token->GetFileOffset();
        it++;
    }

    it = g_Deopt.script_tokens.begin();
    while(it != g_Deopt.script_tokens.end()) {
        QScriptToken *token = *it;
        rewrite_offsets(reinterpret_cast<IStream*>(&ms), token);
        it++;
    }

    QScriptSymbol *symbol = new QScriptSymbol(token_buffer, ms.GetOffset());
    symbol->SetNameChecksum(g_Deopt.root_name_checksum);
    g_Deopt.write_stream->WriteSymbol(symbol);
    delete symbol;

    g_Deopt.script_tokens.clear();
    original_offsets.clear();
    
}
