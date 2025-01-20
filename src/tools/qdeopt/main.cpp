#include <stdio.h>
#include <stdint.h>
#include <map>

#include <FileStream.h>
#include <ISStream.h>
#include <QSymbol.h>
#include <SymbolFileStream.h>
#include <QScriptToken.h>
#include <ChecksumNameToken.h>
#include <dbginfo.h>


void WriteSymbolAsScriptToken(QSymbol *symbol, IStream *stream);

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


    if(!fsout.IsFileOpened()) {
        fprintf(stderr, "Failed to open file: %s\n", argv[2]);
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

    QSymbol *symbol;
    int x = 0;
    while(fs.GetOffset() < total_length) {
        symbol = ss.NextSymbol();
        if(symbol == nullptr) {
            break;
        }
        WriteSymbolAsScriptToken(symbol, &fsout);        
    }
    
    const char* dbgcache_path = getenv("QBTOOLS_DBGINFO_PATH");
    if (dbgcache_path != NULL) {
        printf("** loading dbginfo: %s\n", dbgcache_path);
        dbginfo_load_cache(dbgcache_path);
    }
    else {
        printf("** no dbg data specified\n");
    }
    

    std::map<uint32_t, const char *>::iterator it = m_checksum_names.begin();
    while(it != m_checksum_names.end()) {
        std::pair<uint32_t, const char*> p = *it;
        it++;
        p.second = dbginfo_resolve(p.first);
        if(p.first == 0 || p.second == NULL)
            continue;
        ChecksumNameToken token(p.first, p.second);
        token.Write(&fsout);
    }
    fsout.WriteByte(ESCRIPTTOKEN_ENDOFFILE);

    return 0;
}