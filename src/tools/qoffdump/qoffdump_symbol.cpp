#include "main.h"

#include <FileStream.h>
#include <ISStream.h>
#include <QSymbolToken.h>
#include <SymbolFileStream.h>
#include <QScriptToken.h>
#include <ChecksumNameToken.h>
#include <dbginfo.h>
#include <QScriptSymbol.h>

#include <MemoryStream.h>
#include <InlinePackStructToken.h>


void handle_script(QScriptSymbol *qscript) {
    MemoryStream ms(qscript->GetDecompBuff(), qscript->GetDecompLen());
    ms.SetReadEndian(ISTREAM_LITTLE_ENDIAN);

    #if 0
    printf("dumping script: %08x\n", qscript->GetNameChecksum());
    char name[256];
    sprintf(name, "%08x.bin", qscript->GetNameChecksum());
    FILE *rfd = fopen(name, "r");
    if(rfd) {
        fclose(rfd);
    } else {
        FILE *fd = fopen(name, "wb");
        if(fd) {
            fwrite(qscript->GetDecompBuff(), qscript->GetDecompLen(), 1, fd);
            fclose(fd);
        }
    }
    #endif
    

    while(qscript->GetDecompLen() > ms.GetOffset()) {
        uint32_t read_offset = ms.GetOffset(); //skip type(1) + keyword script byte(1), name type(1)+value(4)
        uint8_t type = ms.ReadByte();
        QScriptToken *token = QScriptToken::Resolve(type);
        assert(token);

        if(token->GetType() == ESCRIPTTOKEN_INLINEPACKSTRUCT) {
            size_t inline_offset = ms.GetOffset() + 2;
            int padding = (sizeof(uint32_t)) - (((inline_offset - 1) % sizeof(uint32_t)) + 1);
            assert(padding <= 4);
            InlinePackStructToken* ip = reinterpret_cast<InlinePackStructToken*>(token);
            ip->SetPadding(padding);            
        }

        token->SetFileOffset(ms.GetOffset()-sizeof(uint8_t));
        token->LoadParams(&ms);

        g_tokens.push_back(token);
    }

    std::vector<QScriptToken *>::iterator it = g_tokens.begin();
    while(it != g_tokens.end()) {
        QScriptToken *t = *it;
        dump_token_offsets(t);
        it++;
    }
    g_tokens.clear();

}
int main(int argc, const char *argv[]) {
    if(argc  < 2) {
        fprintf(stderr, "usage: %s [inpath]\n",argv[0]);
        return -1;
    }
    FileStream fs(argv[1]);
    fs.SetReadEndian(ISTREAM_BIG_ENDIAN);


    if(!fs.IsFileOpened()) {
        fprintf(stderr, "Failed to open file: %s\n", argv[1]);
        return -1;
    }

    fs.ReadUInt32();
    uint32_t total_length = fs.ReadUInt32();
    fs.ReadUInt32();
    fs.ReadUInt32();
    fs.ReadUInt32();
    fs.ReadUInt32();
    fs.ReadUInt32();

    SymbolFileStream ss = SymbolFileStream(&fs);

    QSymbolToken *symbol;
    int x = 0;
    while(fs.GetOffset() < total_length) {
        symbol = ss.NextSymbol();
        if(symbol == nullptr) {
            break;
        }
        if(symbol->GetType() == ESYMBOLTYPE_QSCRIPT) {
            QScriptSymbol *sym = reinterpret_cast<QScriptSymbol*>(symbol);
            handle_script(sym);
            
        }
        
    }

    return 0;
}