
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
#include <StructureSymbol.h>

EScriptToken next_usable_type(std::vector<QScriptToken *>::iterator begin, std::vector<QScriptToken *>::iterator end) {
    std::vector<QScriptToken *>::iterator it = begin;
    while(it != end) {
        QScriptToken *t = *it;
        switch(t->GetType()) {
            case ESCRIPTTOKEN_ENDOFLINE:
            case ESCRIPTTOKEN_ENDOFLINENUMBER:
            break;
            default:
                return t->GetType();
        }
        it++;
    }
    return ESCRIPTTOKEN_ENDOFFILE;
}
std::vector<QScriptToken *>::iterator read_array_struct_item(std::vector<QScriptToken *>::iterator begin, std::vector<QScriptToken *>::iterator end, QSymbolToken **out) {
    std::vector<QScriptToken *>::iterator it = begin;
    std::vector<QSymbolToken *> children;
    QSymbolToken *child;
    int depth = 1;
    bool in_name_mode = true;
    uint32_t struct_item_name = 0;
    while(it != end && depth > 0) {
        QScriptToken *t = *it;
        switch(t->GetType()) {
            case ESCRIPTTOKEN_EQUALS:
                in_name_mode = false;
            break;
            case ESCRIPTTOKEN_ENDSTRUCT:
                depth--;
            break;
            case ESCRIPTTOKEN_ARGUMENTPACK:
                assert(false);
            break;
            case ESCRIPTTOKEN_STARTSTRUCT:
                it = read_array_struct_item(it+1, end, &child);
                children.push_back(child);
                continue;
            case ESCRIPTTOKEN_NAME:
                if(in_name_mode && next_usable_type(it+1, end) != ESCRIPTTOKEN_EQUALS) {
                    in_name_mode = false;
                    struct_item_name = 0;
                }
                if(in_name_mode) {
                    struct_item_name = reinterpret_cast<NameToken*>(t)->GetChecksum();
                    break;
                }
            case ESCRIPTTOKEN_STRING:    
            case ESCRIPTTOKEN_INTEGER:
            case ESCRIPTTOKEN_FLOAT:
            
                child = ConvertToken(t);
                child->SetNameChecksum(struct_item_name);
                child->SetIsStructItem(true);
                children.push_back(child);
                in_name_mode = true;
        }
        it++;
    }

    *out = new StructureSymbol(children);
    return g_Deopt.script_tokens.erase(begin, it);
}

std::vector<QScriptToken *>::iterator handle_struct_array(uint32_t name, std::vector<QSymbolToken *> &struct_symbols, std::vector<QScriptToken *>::iterator begin, std::vector<QScriptToken *>::iterator end) {
    std::vector<QScriptToken *>::iterator it = begin;

    std::vector<QSymbolToken *> arr_items;

    QSymbolToken *sym;

    while(it != end) {
        QScriptToken *t = *it;
        if(t->GetType() == ESCRIPTTOKEN_STARTARRAY) {
            assert(false);
        } else if(t->GetType() == ESCRIPTTOKEN_ENDARRAY) {
            //printf("** ARRAY got end array\n");
            it++;
            break;
        } else if(t->GetType() == ESCRIPTTOKEN_STARTSTRUCT) {
            //printf("*** READ START STUCT\n");
            it = read_array_struct_item(it+1, end, &sym);
            arr_items.push_back(sym);
            continue;
        } else if(t->GetType() ==ESCRIPTTOKEN_ENDSTRUCT) {
            assert(false);
        } else if(t->GetType() == ESCRIPTTOKEN_ARGUMENTPACK) {
            assert(false);
        }
        sym = ConvertToken(t);
        arr_items.push_back(sym);
        it++;
    }

    uint32_t num_items = arr_items.size();
    QSymbolToken **syms = new QSymbolToken*[num_items];
    for(int i=0;i<num_items;i++) {
        syms[i] = arr_items.at(i);
    }

    ArraySymbol *arr_sym = new ArraySymbol(syms, num_items);
    arr_sym->SetNameChecksum(name);
    arr_sym->SetIsStructItem(true);
    struct_symbols.push_back(arr_sym);

    return g_Deopt.script_tokens.erase(begin, it);
}
void emit_array_of_structs() {
    int depth = 1;

    bool in_name_mode = true;
    uint32_t struct_item_name;
    QScriptToken *struct_value = NULL;

    std::vector<QScriptToken *>::iterator it = g_Deopt.script_tokens.begin();
    std::vector<QSymbolToken *> struct_symbols;
    QSymbolToken *symbol;

    std::vector<StructureSymbol *> arr_syms;

    do {
        QScriptToken *t = *it;
        switch(t->GetType()) {
            case ESCRIPTTOKEN_ENDSTRUCT:
                //printf("ARRAY end struct\n");
                arr_syms.push_back(new StructureSymbol(struct_symbols));
                struct_symbols.clear();
                depth--;
            break;   
            case ESCRIPTTOKEN_STARTSTRUCT:
                //printf("ARRAY start struct\n");
                depth++;
            break;
            case ESCRIPTTOKEN_STARTARRAY:
                //printf("ARRAY start array\n");
                it = handle_struct_array(struct_item_name, struct_symbols, it+1, g_Deopt.script_tokens.end());
                in_name_mode = true;
                continue;
            break;
            case ESCRIPTTOKEN_ENDARRAY:
                depth--;
            break;
            case ESCRIPTTOKEN_EQUALS:
                in_name_mode = false;
            break;
            case ESCRIPTTOKEN_ARGUMENTPACK:
                assert(false);
            break;
            case ESCRIPTTOKEN_NAME:
                if(in_name_mode && next_usable_type(it+1, g_Deopt.script_tokens.end()) != ESCRIPTTOKEN_EQUALS) {
                    in_name_mode = false;
                    struct_item_name = 0;
                }
                if(in_name_mode) {
                    struct_item_name = reinterpret_cast<NameToken*>(t)->GetChecksum();
                    break;
                }
            case ESCRIPTTOKEN_STRING:    
            case ESCRIPTTOKEN_INTEGER:
            case ESCRIPTTOKEN_FLOAT:
                struct_value = t;
                symbol = ConvertToken(struct_value);
                assert(symbol);
                symbol->SetIsStructItem(true);
                symbol->SetNameChecksum(struct_item_name);
                struct_symbols.push_back(symbol);
                in_name_mode = true;
            break;
            default:
                assert(false);
        }
        it = g_Deopt.script_tokens.erase(it);
    } while(depth > 0 && it != g_Deopt.script_tokens.end());


    size_t num_syms = arr_syms.size();
    QSymbolToken **syms = new QSymbolToken*[num_syms];
    for(int i=0;i<num_syms;i++) {
        syms[i] = arr_syms.at(i);
    }
    ArraySymbol sym(syms, num_syms);
    sym.SetNameChecksum(g_Deopt.root_name_checksum);
    g_Deopt.write_stream->WriteSymbol(&sym);

    delete[] syms;
}

void emit_array() {
    if(g_Deopt.script_tokens.empty()) { //TODO: handle empty array
        assert(false);
    }

    switch(g_Deopt.script_tokens.front()->GetType()) {
            case ESCRIPTTOKEN_STARTSTRUCT:
                emit_array_of_structs();
            return;
            case ESCRIPTTOKEN_STARTARRAY:
            assert(false);
            printf("emit array of arrays\n");
            return;
    }
        
    size_t num_tokens = g_Deopt.script_tokens.size();
    QSymbolToken **sym_tokens = new QSymbolToken*[num_tokens];

    for(int i=0;i<num_tokens;i++) {
        QScriptToken *t = g_Deopt.script_tokens.at(i);

        sym_tokens[i] = ConvertToken(t);
        assert(sym_tokens[i]);
    }


    ArraySymbol sym(sym_tokens, num_tokens);
    sym.SetNameChecksum(g_Deopt.root_name_checksum);
    g_Deopt.write_stream->WriteSymbol(&sym);

    delete[] sym_tokens;

    g_Deopt.script_tokens.clear();


}
