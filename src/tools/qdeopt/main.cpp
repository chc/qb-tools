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
        fprintf(stderr, "usage: %s [inpath] [outpath] (dbgpath)\n",argv[0]);
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

    const char* dbgpath = NULL;
    if (argc == 4) {
        dbgpath = argv[3];
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
    
    if (dbgpath != NULL) {
        printf("** loading dbg .dbg file: %s\n", dbgpath);
        dbginfo_load_dbg(dbgpath);
    }

    const char* dbg_pak_path = getenv("QBTOOLS_DBG_PAK");
    const char* dbg_pab_path = getenv("QBTOOLS_DBG_PAB");
    if (dbg_pak_path != NULL) {
        printf("** loading dbg pak\n", dbg_pak_path);
        dbginfo_load_pak(dbg_pak_path, dbg_pab_path);
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