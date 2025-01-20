#include <stdio.h>
#include <stdint.h>
#include <vector>

#include <FileStream.h>
#include <QStream.h>
#include <QScriptToken.h>
#include <FastIfToken.h>
#include <FastElseToken.h>
#include <ElseIfToken.h>
#include <JumpToken.h>
#include <ShortJumpToken.h>
#include <RandomToken.h>
#include <NameToken.h>
#include <cassert>

std::vector<QScriptToken *> g_tokens;

QScriptToken *find_by_offset(size_t offset) {
    std::vector<QScriptToken *>::iterator it = g_tokens.begin();
    while(it != g_tokens.end()) {
        QScriptToken *t = *it;
        if(t->GetFileOffset() == offset) {
            return t;
        }
        it++;
    }
    return nullptr;
}

void dump_fastif(FastIfToken *fif) {
    uint32_t file_offset = fif->GetFileOffset() + sizeof(uint8_t); //skip type
    uint32_t target_offset = fif->GetOffset() + file_offset;
    QScriptToken *p = find_by_offset(target_offset);
    assert(p);

    printf("IF: %08x - %d:%08x\n", fif->GetFileOffset(), p->GetType(), target_offset);
}
void dump_fastelse(FastElseToken *fif) {
    uint32_t file_offset = fif->GetFileOffset() + sizeof(uint8_t); //skip type
    uint32_t target_offset = fif->GetOffset() + file_offset;
    QScriptToken *p = find_by_offset(target_offset);
    assert(p);

    printf("ELSE: %08x - %d:%08x\n", fif->GetFileOffset(), p->GetType(), target_offset);
}
void dump_fastelseif(ElseIfToken *fif) {
    uint32_t file_offset = fif->GetFileOffset() + sizeof(uint8_t); //skip type
    uint32_t target_offset = fif->GetNextOffset() + file_offset;
    QScriptToken *p = find_by_offset(target_offset);
    assert(p);

    printf("ELSEIF NEXT: %08x - %d:%08x\n", fif->GetFileOffset(), p->GetType(), target_offset);

    target_offset = fif->GetEndIfOffset() + file_offset;
    p = find_by_offset(target_offset);
    assert(p);

    printf("ELSEIF END: %08x - %d:%08x\n", fif->GetFileOffset(), p->GetType(), target_offset);
}
void dump_jump(JumpToken *fif) {
    uint32_t file_offset = fif->GetFileOffset() + sizeof(uint8_t); //skip type
    uint32_t target_offset = fif->GetOffset() + file_offset + sizeof(uint32_t);
    QScriptToken *p = find_by_offset(target_offset);
    assert(p);
    printf("JUMP: %08x - %d:%08x\n", fif->GetFileOffset(), p->GetType(), target_offset);
}
void dump_shortjump(ShortJumpToken *fif) {
    uint32_t file_offset = fif->GetFileOffset() + sizeof(uint8_t); //skip type
    uint32_t target_offset = fif->GetOffset() + file_offset;
    QScriptToken *p = find_by_offset(target_offset);

    assert(p);
    printf("SJUMP: %08x - %d:%08x\n", fif->GetFileOffset(), p->GetType(), target_offset);
}
void dump_random(RandomToken *fif) {
    printf("random: %08x\n", fif->GetFileOffset());
    uint32_t num_items = fif->GetNumItems();
    for(int i=0;i<num_items;i++) {
        uint32_t target_offset = fif->GetFileOffset() + fif->CalculateTokenOffset(i) + fif->GetRandomOffset(i);
        QScriptToken *p = find_by_offset(target_offset);        
        assert(p);
        printf("RAND: %08x - %d:%08x\n", fif->GetFileOffset(), p->GetType(), target_offset);
    }
}
void dump_token_offsets(QScriptToken *token) {
    static bool got_script = false;
    switch(token->GetType()) {
        case ESCRIPTTOKEN_KEYWORD_FASTIF:
            dump_fastif(reinterpret_cast<FastIfToken*>(token));
            break;
        case ESCRIPTTOKEN_KEYWORD_FASTELSE:
            dump_fastelse(reinterpret_cast<FastElseToken*>(token));
            break;
        case ESCRIPTTOKEN_KEYWORD_ELSEIF:
            dump_fastelseif(reinterpret_cast<ElseIfToken*>(token));
            break;
        case ESCRIPTTOKEN_KEYWORD_SCRIPT:
            got_script = true;
        break;
        case ESCRIPTTOKEN_NAME:
            if(got_script) {
                got_script = false;
                printf("on script: %08x\n", reinterpret_cast<NameToken*>(token)->GetChecksum());
            }
        break;
        case ESCRIPTTOKEN_JUMP:
            dump_jump(reinterpret_cast<JumpToken*>(token));
            break;
        case ESCRIPTTOKEN_SHORTJUMP:
            dump_shortjump(reinterpret_cast<ShortJumpToken*>(token));
            break;
        case ESCRIPTTOKEN_KEYWORD_RANDOM:
        case ESCRIPTTOKEN_KEYWORD_RANDOM_PERMUTE:
        case ESCRIPTTOKEN_KEYWORD_RANDOM_NO_REPEAT:
            dump_random(reinterpret_cast<RandomToken*>(token));
            break;
    }
}