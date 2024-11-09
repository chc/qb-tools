#include "main.h"

#include <FileStream.h>
#include <ISStream.h>
#include <SymbolFileStream.h>
#include <QStream.h>
#include <QScriptToken.h>
#include <ArgumentPackToken.h>

int main(int argc, const char *argv[]) {
    if(argc  < 2) {
        fprintf(stderr, "usage: %s [inpath]\n",argv[0]);
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
        uint32_t offset = fs.GetOffset();
        token = qs.NextToken();
        token->SetFileOffset(offset);
        if(token == NULL || token->GetType() == ESCRIPTTOKEN_ENDOFFILE) {
            break;
        }
        if(token->GetType() == ESCRIPTTOKEN_ARGUMENTPACK) {
            ArgumentPackToken *apt = reinterpret_cast<ArgumentPackToken*>(token);
            apt->LoadExtendedParams(&fs);
        }
        g_tokens.push_back(token);
    }

    std::vector<QScriptToken *>::iterator it = g_tokens.begin();
    while(it != g_tokens.end()) {
        dump_token_offsets(*it);
        it++;
    }
    return 0;
}