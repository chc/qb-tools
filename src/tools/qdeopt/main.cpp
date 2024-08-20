#include <stdio.h>
#include <stdint.h>
#include <map>

#include <FileStream.h>
#include <ISStream.h>
#include <QSymbolToken.h>
#include <SymbolFileStream.h>
#include <QScriptToken.h>
#include <ChecksumNameToken.h>
#include <dbginfo.h>
void WriteSymbolAsScriptToken(QSymbolToken *symbol, IStream *stream);

std::map<uint32_t, const char *> m_checksum_names;

int main(int argc, const char *argv[]) {
    if(argc  < 3) {
        fprintf(stderr, "usage: %s [inpath] [outpath]\n",argv[0]);
        return -1;
    }
    FileStream fs(argv[1]);
    fs.SetReadEndian(ISTREAM_SYMBOL_ENDIAN);


    if(!fs.IsFileOpened()) {
        fprintf(stderr, "Failed to open file: %s\n", argv[1]);
        return -1;
    }

    FileStream fsout(argv[2], true);


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
        WriteSymbolAsScriptToken(symbol, &fsout);        
    }

    const char *dbginfo_path = getenv("QTOOLS_DBGPAK_PATH");
    if(dbginfo_path != NULL) {
        printf("** loading dbginfo path: %s\n", dbginfo_path);
        dbginfo_load(dbginfo_path);
    } else {
        printf("** checksum path not specified\n");
    }
    

    std::map<uint32_t, const char *>::iterator it = m_checksum_names.begin();
    while(it != m_checksum_names.end()) {
        it++;
        it->second = dbginfo_resolve(it->first);
        if(it->first == 0 || it->second == NULL)
            continue;
        ChecksumNameToken token(it->first, it->second);
        token.Write(&fsout);
    }
    fsout.WriteByte(ESCRIPTTOKEN_ENDOFFILE);

    return 0;
}