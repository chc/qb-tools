#include <stdio.h>
#include <stdint.h>
#include <QStream.h>
#include <QScriptToken.h>

#include <ArgToken.h>
#include <NameToken.h>
#include <ChecksumNameToken.h>
#include <JumpToken.h>
#include <InjectedToken.h>
#include <ArgumentPackToken.h>

#include <vector>
#include <map>
#include <crc32.h>
#include <dbginfo.h>
std::vector<QScriptToken *> token_list;
void dump_token_list(std::vector<QScriptToken *> token_list, FILE *out);

uint32_t gen_checksum(const char *name) {
    char *cpy = strdup(name);
    for(int i=0;i<strlen(cpy);i++) {
        cpy[i] = tolower(cpy[i]);
    }

    uint32_t checksum = crc32(-1, cpy, strlen(cpy));
    free((void *)cpy);
    return checksum;
}


void map_checksum_names() {
    std::map<uint32_t, ChecksumNameToken*> checksum_map;
    std::vector<QScriptToken*>::iterator it = token_list.begin();
    while (it != token_list.end()) {
        QScriptToken* token = *it;
        if (token->GetType() == ESCRIPTTOKEN_CHECKSUM_NAME) {
            ChecksumNameToken* c = reinterpret_cast<ChecksumNameToken *>(token);
            checksum_map[c->GetChecksum()] = c;
        }
        it++;
    }


    it = token_list.begin();    
    while(it != token_list.end()) {
        QScriptToken *token = *it;
        if(token->GetType() == ESCRIPTTOKEN_NAME) {
            NameToken *name = reinterpret_cast<NameToken *>(token);
            ChecksumNameToken *c = checksum_map[name->GetChecksum()];
            if(c) {
                name->SetChecksumName(c);
            }
        }
        it++;
    }

    const char* dbgcache_path = getenv("QBTOOLS_DBGINFO_PATH");
    if (dbgcache_path != NULL) {
        dbginfo_load_cache(dbgcache_path);

        it = token_list.begin();
        while (it != token_list.end()) {
            QScriptToken* token = *it;
            if (token->GetType() == ESCRIPTTOKEN_NAME) {
                NameToken* name = reinterpret_cast<NameToken*>(token);
                if (name->GetName() == nullptr) {
                    const char* resolved_name = dbginfo_resolve(name->GetChecksum());
                    if (resolved_name != nullptr) {
                        ChecksumNameToken* c = new ChecksumNameToken(name->GetChecksum(), resolved_name);
						name->SetChecksumName(c);
						checksum_map[name->GetChecksum()] = c;
                    }
                }
            }
            it++;
        }
    }
}

std::vector<TokenInjection> active_injections;
void append_injections(IStream &fs, QScriptToken *token) {
    size_t offset = fs.GetOffset();
    std::vector<TokenInjection> injections = token->GetInjections();
    if(!injections.empty()) {
        std::vector<TokenInjection>::iterator it = injections.begin();
        while(it != injections.end()) {
            TokenInjection injection = *it;
            if(injection.use_next_jump_offset == false) {
                injection.offset += offset;
            }
            active_injections.push_back(injection);
            it++;
        }
    }
}
void update_jump_injection(IStream &fs, JumpToken *token) {
    size_t offset = fs.GetOffset();
    if(!active_injections.empty()) {
        std::vector<TokenInjection>::iterator it = active_injections.begin();
        while(it != active_injections.end()) {
            if(it->use_next_jump_offset == true) {
                it->use_next_jump_offset = false;
                it->offset = offset + token->GetOffset();
            }
            it++;
        }
    }
}
void perform_injections(IStream &fs) {
    std::vector<TokenInjection>::iterator it = active_injections.begin();
    while(it != active_injections.end()) {
        TokenInjection injection = *it;
        if(fs.GetOffset() == injection.offset && injection.use_next_jump_offset == false) {
            InjectedToken *token = new InjectedToken(injection.token);
            token_list.push_back(token);

            //its assumed there will only be 1 injection at a given offset
            active_injections.erase(it);
            it = active_injections.begin();
            continue;
            //break;
        }
        it++;
    }
}

int main(int argc, const char *argv[]) {
    if(argc  < 2) {
        fprintf(stderr, "usage: %s [filepath]\n",argv[0]);
        return -1;
    }

    FileStream fs(argv[1]);

    QStream qs = QStream(&fs);

    if(!fs.IsFileOpened()) {
        fprintf(stderr, "Failed to open file: %s\n", argv[1]);
        return -1;
    }

    QScriptToken *token;
    while(true) {
        token = qs.NextToken();
        if(token != NULL) {
            append_injections(fs, token);
            if(token->GetType() == ESCRIPTTOKEN_JUMP) {
                update_jump_injection(fs, reinterpret_cast<JumpToken*>(token));
            }
            if(token->GetType() == ESCRIPTTOKEN_ARGUMENTPACK) {
                reinterpret_cast<ArgumentPackToken*>(token)->LoadExtendedParams(&fs);
            }
            token_list.push_back(token);
            perform_injections(fs);
        }
        if(token == NULL || token->GetType() == ESCRIPTTOKEN_ENDOFFILE) {
            break;
        }

    }
    map_checksum_names();

    dump_token_list(token_list, stdout);
    return 0;
}