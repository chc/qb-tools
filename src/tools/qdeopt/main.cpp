#include <stdio.h>
#include <stdint.h>

#include <FileStream.h>
#include <ISStream.h>
#include <QSymbolToken.h>
#include <SymbolFileStream.h>
#include <QScriptToken.h>
void WriteSymbolAsScriptToken(QSymbolToken *symbol, IStream *stream);

extern int32_t g_last_script_keyword;

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
    fsout.WriteByte(ESCRIPTTOKEN_ENDOFFILE);

    return 0;
}