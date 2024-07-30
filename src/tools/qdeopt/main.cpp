#include <stdio.h>
#include <stdint.h>
#include <map>

#include <FileStream.h>
#include <ISStream.h>
#include <QSymbolToken.h>
#include <SymbolFileStream.h>
#include <QScriptToken.h>
#include <ChecksumNameToken.h>
void WriteSymbolAsScriptToken(QSymbolToken *symbol, IStream *stream);

extern int32_t g_last_script_keyword;

std::map<uint32_t, const char *> m_checksum_names;

typedef struct {
    uint32_t checksum;
    char checksum_name[64];
} ChecksumTableEntry;
void LoadChecksums(const char *path) {
    ChecksumTableEntry entry;
    FILE *fd = fopen(path, "rb");
    if(!fd) {
        return;
    }
    while(true) {
        if(feof(fd)) {
            break;
        }
        int len = fread(&entry, sizeof(ChecksumTableEntry), 1, fd);
        if(len != 1) {
            break;
        }
        if(m_checksum_names.find(entry.checksum) == m_checksum_names.end()) {
            m_checksum_names[entry.checksum] = entry.checksum_name;
        }
    }
    fclose(fd);
}

int main(int argc, const char *argv[]) {
    if(argc  < 3) {
        fprintf(stderr, "usage: %s [inpath] [outpath]\n",argv[0]);
        return -1;
    }
    FileStream fs(argv[1]);
    fs.SetReadEndian(ISTREAM_BIG_ENDIAN);


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

    LoadChecksums("checksums.bin");

    std::map<uint32_t, const char *>::iterator it = m_checksum_names.begin();
    while(it != m_checksum_names.end()) {
        std::pair<uint32_t, const char *> p = *it;
        it++;
        if(p.first == 0)
            continue;
        ChecksumNameToken token(p.first, p.second);
        token.Write(&fsout);
    }
    fsout.WriteByte(ESCRIPTTOKEN_ENDOFFILE);

    return 0;
}