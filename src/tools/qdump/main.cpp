#include <stdio.h>
#include <stdint.h>
#include "QScriptFileStream.h"
#include "token/QScriptToken.h"

#include "token/NameToken.h"
#include "token/ChecksumNameToken.h"
#include "token/InjectedToken.h"

#include <vector>
const char *path = "/Users/chc/skate5/data/scripts/game/animevents.qb";
std::vector<QScriptToken *> token_list;
void dump_token_list(std::vector<QScriptToken *> token_list, FILE *out);

ChecksumNameToken *resolve_name(uint32_t checksum) {
    std::vector<QScriptToken *>::iterator it = token_list.begin();
    while(it != token_list.end()) {
        QScriptToken *token = *it;
        if(token->GetType() == ESCRIPTTOKEN_CHECKSUM_NAME) {
            ChecksumNameToken *c = reinterpret_cast<ChecksumNameToken *>(token);
            if(c && c->GetChecksum() == checksum) {
                return c;
            }
        }
        it++;
    }
    return NULL;
}

void map_checksum_names() {
    std::vector<QScriptToken *>::iterator it = token_list.begin();
    while(it != token_list.end()) {
        QScriptToken *token = *it;
        if(token->GetType() == ESCRIPTTOKEN_NAME) {
            NameToken *name = reinterpret_cast<NameToken *>(token);
            ChecksumNameToken *c = resolve_name(name->GetChecksum());
            if(c) {
                name->SetChecksumName(c);
            }
        }
        it++;
    }
}

std::vector<TokenInjection> active_injections;
void append_injections(QScriptFileStream &fs, QScriptToken *token) {
    size_t offset = fs.GetOffset();
    std::vector<TokenInjection> injections = token->GetInjections();
    if(!injections.empty()) {
        std::vector<TokenInjection>::iterator it = injections.begin();
        while(it != injections.end()) {
            TokenInjection injection = *it;
            injection.offset += offset;
            active_injections.push_back(injection);
            it++;
        }
    }
}
void perform_injections(QScriptFileStream &fs) {
    std::vector<TokenInjection>::iterator it = active_injections.begin();
    while(it != active_injections.end()) {
        TokenInjection injection = *it;
        if(fs.GetOffset() == injection.offset) {
            InjectedToken *token = new InjectedToken(injection.token);
            token_list.push_back(token);
        }
        it++;
    }
}
int main() {
    QScriptFileStream fs(path);

    QScriptToken *token;
    while(true) {
        token = fs.NextToken();
        if(token != NULL) {
            append_injections(fs, token);
            token_list.push_back(token);
            perform_injections(fs);
        }
        if(token == NULL || token->GetType() == ESCRIPTTOKEN_ENDOFFILE) {
            break;
        }

        if(token->GetType() == ESCRIPTTOKEN_ENDOFLINE) {
            break;
        }

    }
    map_checksum_names();

    dump_token_list(token_list, stdout);
    return 0;
}