#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint32_t is_processed;
    uint32_t total_length;
    uint32_t unknown;
    uint32_t unknown2;
    uint32_t unknown3;
    uint32_t unknown4;
    uint32_t unknown5;
} SymbolDumpHeader;

const char *path = "/Users/chc/thpg/SCRIPTS/ENGINE/ENGINE_PARAMS.QB.PS3";
int main(int argc, const char *argv[]) {
    if(argc  < 2) {
        fprintf(stderr, "usage: %s [filepath]\n",argv[0]);
        return -1;
    }
    printf("size: %d\n", sizeof(SymbolDumpHeader));

    return 0;
}