#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <QSymbol.h>
#include "engine.h"
#include "cfuncs.h"
#include <SymbolFileStream.h>

/*
    args:
        --pak qb.pak --pab qb.pab
        --qb test.qb
*/



int main() {
    QEngine *eng = new QEngine();
    const char *path = "run.qb";
    FileStream fs(path);
    fs.SetReadEndian(ISTREAM_SYMBOL_ENDIAN);
    if(!fs.IsFileOpened()) {
        fprintf(stderr, "Failed to open file: %s\n", path);
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
    uint32_t src_file_checksum = 0;
    int x = 0;
    while(fs.GetOffset() < total_length) {
        symbol = ss.NextSymbol();
        if(symbol == nullptr) {
            break;
        }
        eng->load_symbol(symbol);
    }

    eng->register_cfunc(0x2de8c60e, CFuncs::printf);
    eng->run_script(0x9ead52f7);
    
    return 0;
}