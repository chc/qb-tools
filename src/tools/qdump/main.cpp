#include <stdio.h>
#include <stdint.h>
#include "QScriptFileStream.h"
#include "token/QScriptToken.h"

#include "token/NameToken.h"
#include "token/ChecksumNameToken.h"

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

int main() {
    QScriptFileStream fs(path);

    QScriptToken *token;
    while(true) {
        token = fs.NextToken();
        if(token != NULL) {
            token_list.push_back(token);
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