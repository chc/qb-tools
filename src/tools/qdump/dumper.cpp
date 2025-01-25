#include <stdio.h>
#include <stdint.h>
#include "QStream.h"
#include <QScriptToken.h>

#include <EndOfLineNumberToken.h>
#include <NameToken.h>
#include <ChecksumNameToken.h>

#include <vector>
#include <cassert>

int current_line_number = 0;
int current_tab_index = 0;
bool removelaststructtab = true;
bool removelastarraytab = true;
bool dump_endofline_numbers = false;

int get_line_number(std::vector<QScriptToken *> lines) {
    std::vector<QScriptToken *>::iterator it = lines.begin();
    while(it != lines.end()) {
        QScriptToken *t = *it;
        if(t->GetType() == ESCRIPTTOKEN_ENDOFLINENUMBER) {
            EndOfLineNumberToken *eol = reinterpret_cast<EndOfLineNumberToken *>(t);
            return eol->GetLineNumber();
        }
        it++;
    }
    return 0;
}

void emit_token(QScriptToken *token, FILE *out) {
    std::string e = token->ToString();
    if (token->GetType() == ESCRIPTTOKEN_STARTARRAY || token->GetType() == ESCRIPTTOKEN_STARTSTRUCT) {
        current_tab_index++;
    }
    if (token->GetType() == ESCRIPTTOKEN_ENDARRAY) {
        if (removelastarraytab) {
            current_tab_index--;
        } else {
            removelastarraytab = true;
        }
    }
    if (token->GetType() == ESCRIPTTOKEN_ENDSTRUCT) {
        if (removelaststructtab) {
            current_tab_index--;
        } else {
            removelaststructtab = true;
        }
    }
    fprintf(out, "%s", e.c_str());
}

int get_pretab_offset(std::vector<QScriptToken *> lines) {
    int index = 0;
    std::vector<QScriptToken *>::iterator it = lines.begin();
    while(it != lines.end()) {
        QScriptToken *t = *it;
        index += t->GetPreTabOffset();
        it++;
    }
    return index;
}
int get_posttab_offset(std::vector<QScriptToken *> lines) {
    int index = 0;
    std::vector<QScriptToken *>::iterator it = lines.begin();
    while(it != lines.end()) {
        QScriptToken *t = *it;
        index += t->GetPostTabOffset();
        it++;
    }
    return index;
}

void emit_line(std::vector<QScriptToken *> lines, FILE *out) {
    int line_number = get_line_number(lines);
    if (dump_endofline_numbers) { //one of the EOL chars is always emitted by the token ToString
        int diff = line_number - current_line_number - 1;
        while (diff > 0 && diff--) {
            fprintf(out, "\n");
        }
    }


    int pretab = get_pretab_offset(lines) + current_tab_index;
    if (pretab < 0) {
        pretab = 0;
    }
    //assert(pretab >= 0);
    current_tab_index = pretab;
    while(pretab > 0 && pretab--) {
        fprintf(out, "\t");
    }
    
    std::vector<QScriptToken *>::iterator it = lines.begin();
    while(it != lines.end()) {
        QScriptToken *token = *it;
        emit_token(token, out);
        it++;
    }
    current_line_number = line_number;

    int posttab = get_posttab_offset(lines) + current_tab_index;
    if (posttab < 0) {
        posttab = 0;
    }
    //assert(posttab >= 0);
    current_tab_index = posttab;
}

bool is_end_of_line_token(QScriptToken *token) {
    switch(token->GetType()) {
        case ESCRIPTTOKEN_ENDOFLINE:
        case ESCRIPTTOKEN_ENDOFLINENUMBER:
        case ESCRIPTTOKEN_ENDOFFILE:
            return true;
    }
    return false;
}

bool exclude_space_after_by_this_token(QScriptToken *token) {
    switch (token->GetType()) {
        //case ESCRIPTTOKEN_ENDOFFILE:
        //case ESCRIPTTOKEN_ENDOFLINE:
        //case ESCRIPTTOKEN_ENDOFLINENUMBER:
        case ESCRIPTTOKEN_OPENPARENTH:
        case ESCRIPTTOKEN_CHECKSUM_NAME:
        case ESCRIPTTOKEN_COLON:
        case ESCRIPTTOKEN_DOT:
        case ESCRIPTTOKEN_STARTARRAY:
        case ESCRIPTTOKEN_STARTSTRUCT:
        case ESCRIPTTOKEN_KEYWORD_RANDOM_RANGE:
        case ESCRIPTTOKEN_KEYWORD_RANDOM_RANGE2:
        case ESCRIPTTOKEN_KEYWORD_RANDOMFLOAT:
        case ESCRIPTTOKEN_KEYWORD_RANDOMINTEGER:
            return true;
    }
    return false;
}

bool exclude_space_after_by_next_token(QScriptToken *token) {
    switch (token->GetType()) {
        case ESCRIPTTOKEN_ENDOFLINE:
        case ESCRIPTTOKEN_ENDOFLINENUMBER:
        case ESCRIPTTOKEN_CLOSEPARENTH:
        case ESCRIPTTOKEN_COLON:
        case ESCRIPTTOKEN_COMMA:
        case ESCRIPTTOKEN_DOT:
        case ESCRIPTTOKEN_ENDARRAY:
        case ESCRIPTTOKEN_ENDSTRUCT:
            return true;
    }
    return false;
}
//space handled special cases
//ESCRIPTTOKEN_ARGUMENTPACK unless THDJ WII then not handled
//ESCRIPTTOKEN_JUMP and random count > 0
//ESCRIPTTOKEN_KEYWORD_RANDOM
//ESCRIPTTOKEN_KEYWORD_RANDOM2
//ESCRIPTTOKEN_KEYWORD_RANDOM_PERMUTE
//ESCRIPTTOKEN_KEYWORD_RANDOM_NO_REPEAT
//


void dump_token_list(std::vector<QScriptToken *> token_list, FILE *out) {
    std::vector<QScriptToken *> current_line;
    std::vector<QScriptToken *>::iterator it = token_list.begin();
    
    while(it != token_list.end()) {
        QScriptToken *token = *it;
        bool eol = is_end_of_line_token(token);
        
        if (eol || exclude_space_after_by_this_token(token)) {
        } else if ((it + 1) != token_list.end() && exclude_space_after_by_next_token(*(it + 1))) {
        } else if ((it + 1) != token_list.end() && (*(it + 1))->GetType() == ESCRIPTTOKEN_STARTARRAY && !(token->GetType() == ESCRIPTTOKEN_EQUALS)) {
        } else {
            token->AppendSpaceToString = true;
        }
        
        if (token->GetType() == ESCRIPTTOKEN_ENDARRAY && current_line.empty()) {
            current_tab_index--;
            removelastarraytab = false;
        }
        if (token->GetType() == ESCRIPTTOKEN_ENDSTRUCT && current_line.empty()) {
            current_tab_index--;
            removelaststructtab = false;
        }
        
        current_line.push_back(token);
        if (eol) {
            emit_line(current_line, out);
            current_line.clear();
        }
        it++;
    }
    if(!current_line.empty()) {
        emit_line(current_line, out);
    }
}
