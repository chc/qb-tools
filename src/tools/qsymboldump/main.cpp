#include <stdio.h>
#include <stdint.h>

#include <FileStream.h>
#include "symbol/ISStream.h"
#include "symbol/QSymbolToken.h"
#include "SStream.h"

int main(int argc, const char *argv[]) {
    if(argc  < 2) {
        fprintf(stderr, "usage: %s [filepath]\n",argv[0]);
        return -1;
    }
    FileStream fs(argv[1]);


    if(!fs.IsFileOpened()) {
        fprintf(stderr, "Failed to open file: %s\n", argv[1]);
        return -1;
    }

    

    //printf("IsProcessed: %d\n", fs.ReadUInt32());
    fs.ReadUInt32();
    uint32_t total_length = fs.ReadUInt32();
    fs.ReadUInt32();
    fs.ReadUInt32();
    fs.ReadUInt32();
    fs.ReadUInt32();
    fs.ReadUInt32();
    /*printf("Total Length: %d\n", total_length);
    printf("unknown: %08x\n", fs.ReadUInt32());
    printf("unknown: %08x\n", fs.ReadUInt32());
    printf("unknown: %08x\n", fs.ReadUInt32());
    printf("unknown: %08x\n", fs.ReadUInt32());
    printf("unknown: %08x\n", fs.ReadUInt32());
    printf("offset: %d\n", fs.GetOffset());*/

    SStream ss = SStream(&fs);

    QSymbolToken *symbol;
    while(fs.GetOffset() < total_length) {
        symbol = ss.NextSymbol();
        if(symbol == nullptr) {
            break;
        }
        //std::string s = symbol->ToString();
        //printf("symbol: %s - %04x\n", s.c_str(), fs.GetOffset());

        if(symbol->GetType() != ESYMBOLTYPE_QSCRIPT) {
            fs.ReadInt32(); //??
        }
        
    }
    printf("exit\n");

    return 0;
}