#include <stdio.h>
#include <stdint.h>
#include "QScriptFileStream.h"
#include "token/QScriptToken.h"

#include "token/EndOfLineNumberToken.h"
#include "token/NameToken.h"
#include "token/ChecksumNameToken.h"

#include <vector>
#include <cassert>

int current_line_number = 0;
int current_tab_index = 0;

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
    int diff = line_number - current_line_number - 1;
    while(diff > 0 && diff--) {
        fprintf(out, "\n");
    }

    int pretab = get_pretab_offset(lines) + current_tab_index;
    assert(pretab >= 0);
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
    assert(posttab >= 0);
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


void dump_token_list(std::vector<QScriptToken *> token_list, FILE *out) {
    std::vector<QScriptToken *> current_line;

    std::vector<QScriptToken *>::iterator it = token_list.begin();
    while(it != token_list.end()) {
        QScriptToken *token = *it;
        bool eol = is_end_of_line_token(token);
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
