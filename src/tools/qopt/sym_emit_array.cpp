
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

#include <ReferenceItemSymbol.h>

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
    bool in_argument_pack = false;
    uint32_t struct_item_name = 0;
    while(it != end && depth > 0) {
        QScriptToken *t = *it;
        switch(t->GetType()) {
            case ESCRIPTTOKEN_EQUALS:
                in_name_mode = false;
            break;
            case ESCRIPTTOKEN_ENDSTRUCT:
                depth--;
                assert(depth==0);
            break;
            case ESCRIPTTOKEN_ARGUMENTPACK:
                in_argument_pack = true;
                it++;
                continue;
            break;
            case ESCRIPTTOKEN_STARTSTRUCT:
                it = read_array_struct_item(it+1, end, &child);
                children.push_back(child);
                in_name_mode = true;
                in_argument_pack = false;
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
            case ESCRIPTTOKEN_PAIR:
            case ESCRIPTTOKEN_VECTOR:
            
                child = ConvertToken(t, in_argument_pack);
                child->SetNameChecksum(struct_item_name);
                child->SetIsStructItem(true);
                children.push_back(child);
                in_name_mode = true;
                in_argument_pack = false;
        }
        it++;
    }

    *out = new StructureSymbol(children);
    return it;
    //return g_Deopt.script_tokens.erase(begin, it);
}

std::vector<QScriptToken *>::iterator handle_struct_array(uint32_t name, std::vector<QSymbolToken *> &struct_symbols, std::vector<QScriptToken *>::iterator begin, std::vector<QScriptToken *>::iterator end) {
    std::vector<QScriptToken *>::iterator it = begin;

    std::vector<QSymbolToken *> arr_items;

    ArraySymbol *arr_sym;
    std::vector<QSymbolToken *> child_arr_items;

    QSymbolToken *sym;

    int depth = 1;
    bool make_reference = false;
    while(it != end && depth) {
        QScriptToken *t = *it;
        if(t->GetType() == ESCRIPTTOKEN_STARTARRAY) {
            it = handle_struct_array(name, child_arr_items, it+1, end);
            arr_sym = new ArraySymbol(child_arr_items);
            arr_items.push_back(arr_sym);
            continue;
        } else if(t->GetType() == ESCRIPTTOKEN_ENDARRAY) {
            depth--;
            assert(depth==0);
            it++;
            break;
        } else if(t->GetType() == ESCRIPTTOKEN_STARTSTRUCT) {
            it = read_array_struct_item(it+1, end, &sym);
            arr_items.push_back(sym);
            continue;
        } else if(t->GetType() ==ESCRIPTTOKEN_ENDSTRUCT) {
            depth--;
            assert(depth==0);
            it++;
            break;
        } else if(t->GetType() == ESCRIPTTOKEN_ARGUMENTPACK) {
            make_reference = true;
            it++;
            continue;
        } else if(is_end_of_line_token(t)) {
            it++;
            continue;
        }
        sym = ConvertToken(t, make_reference);
        make_reference = false;
        arr_items.push_back(sym);
        it++;
    }

    uint32_t num_items = arr_items.size();
    QSymbolToken **syms = new QSymbolToken*[num_items];
    for(int i=0;i<num_items;i++) {
        syms[i] = arr_items.at(i);
    }

    arr_sym = new ArraySymbol(syms, num_items);
    arr_sym->SetNameChecksum(name);
    arr_sym->SetIsStructItem(true);
    struct_symbols.push_back(arr_sym);

    return it;

    //return g_Deopt.script_tokens.erase(begin, it);
}
void emit_array_of_structs() {
    int depth = 1;

    bool in_name_mode = true;
    bool in_ref_mode = false;
    uint32_t struct_item_name = 0;
    QScriptToken *struct_value = NULL;

    std::vector<QScriptToken *>::iterator it = g_Deopt.script_tokens.begin();
    std::vector<QSymbolToken *> struct_symbols;
    QSymbolToken *symbol;

    std::vector<StructureSymbol *> arr_syms;

    do {
        QScriptToken *t = *it;
        switch(t->GetType()) {
            case ESCRIPTTOKEN_ENDSTRUCT:
                arr_syms.push_back(new StructureSymbol(struct_symbols));
                struct_symbols.clear();
                depth--;
            break;   
            case ESCRIPTTOKEN_STARTSTRUCT:
                depth++;
            break;
            case ESCRIPTTOKEN_STARTARRAY:
                it = g_Deopt.script_tokens.erase(handle_struct_array(struct_item_name, struct_symbols, it+1, g_Deopt.script_tokens.end()));
                in_name_mode = true;
                continue;
            break;
            case ESCRIPTTOKEN_ENDARRAY:
                depth--;
                assert(depth==0);
            break;
            case ESCRIPTTOKEN_EQUALS:
                in_name_mode = false;
            break;
            case ESCRIPTTOKEN_ARGUMENTPACK:
                assert(in_ref_mode == false);
                in_ref_mode = true;
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
            case ESCRIPTTOKEN_LOCALSTRING:
            case ESCRIPTTOKEN_INTEGER:
            case ESCRIPTTOKEN_FLOAT:
            case ESCRIPTTOKEN_PAIR:
            case ESCRIPTTOKEN_VECTOR:
                struct_value = t;
                symbol = ConvertToken(struct_value, in_ref_mode);
                in_ref_mode = false;
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

void emit_array_of_arrays() {
    int depth = 1;

    bool in_name_mode = true;
    bool in_ref_mode = false;
    uint32_t struct_item_name = 0;
    QScriptToken *struct_value = NULL;

    std::vector<QScriptToken *>::iterator it = g_Deopt.script_tokens.begin();

    QSymbolToken *symbol;

    std::vector<QSymbolToken *> child_symbols;
    ArraySymbol *arr_sym;
    std::vector<ArraySymbol *> arr_syms;

    do {
        QScriptToken *t = *it;
        switch(t->GetType()) {
            case ESCRIPTTOKEN_ENDARRAY:
                depth--;
                assert(depth==0);
            break;
            case ESCRIPTTOKEN_STARTARRAY:
                it = handle_struct_array(0, child_symbols, it+1, g_Deopt.script_tokens.end());
                assert(child_symbols.size() == 1);
                symbol = child_symbols.front();
                child_symbols.clear();
                
                arr_sym = reinterpret_cast<ArraySymbol*>(symbol);
                arr_syms.push_back(arr_sym);
                continue;
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

    g_Deopt.script_tokens.clear();
}

void emit_array() {
    if(g_Deopt.script_tokens.empty()) {
        ArraySymbol sym(nullptr, 0);
        sym.SetNameChecksum(g_Deopt.root_name_checksum);
        g_Deopt.write_stream->WriteSymbol(&sym);
        return;
    }

    switch(g_Deopt.script_tokens.front()->GetType()) {
            case ESCRIPTTOKEN_STARTSTRUCT:
                emit_array_of_structs();
            return;
            case ESCRIPTTOKEN_STARTARRAY:
                emit_array_of_arrays();
            return;
    }

    bool in_ref_mode = false;
    std::vector<QSymbolToken *> array_tokens;
    for(int i=0;i<g_Deopt.script_tokens.size();i++) {
        QScriptToken *t = g_Deopt.script_tokens.at(i);

        if(t->GetType() == ESCRIPTTOKEN_ARGUMENTPACK) {
            assert(in_ref_mode == false);
            in_ref_mode = true;
            continue;
        }

        QSymbolToken *converted_token = ConvertToken(t, in_ref_mode);
        assert(converted_token);
        array_tokens.push_back(converted_token);
        in_ref_mode = false;
    }


    ArraySymbol sym(array_tokens);
    sym.SetNameChecksum(g_Deopt.root_name_checksum);
    g_Deopt.write_stream->WriteSymbol(&sym);

    g_Deopt.script_tokens.clear();


}
