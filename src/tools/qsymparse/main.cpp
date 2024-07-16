#include <stdio.h>
#include <stdint.h>

#include "symbol.h"
#include <cassert>

void show_dump(unsigned char* data, unsigned int len, FILE* stream);


#define _BYTE_SWAP

#if defined(_BYTE_SWAP)
#define READ_UINT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))
#define READ_UINT16(x) ((x>>8) | (x<<8))
#define READ_INT32(x) ((int32_t)READ_UINT32((uint32_t)x))
#define READ_INT16(x) ((int16_t)READ_UINT16((uint16_t)x))
#else
#define READ_UINT32(x) x
#define READ_UINT16(x) x
#define READ_INT32(x)  x
#endif

typedef struct _ScriptHeader {
    uint32_t unk; //0
    uint32_t total_length; //4
    uint32_t unk2[5]; //8
    //28
} ScriptHeader;
typedef struct _CSymbolTableEntry {
	uint8_t unk[2]; //0, 1
	uint8_t type; //2
	uint8_t unk2; //3
	uint32_t checksum; //4
	uint32_t sourceFileNameChecksum;
	uint32_t union_type; //12
	uint32_t next;
} CSymbolTableEntry;

typedef struct _CSymbolQEntry {
    CSymbolTableEntry table;
    uint32_t checksum;
    uint32_t uncompressed_size;
    uint32_t compressed_size;
} CSymbolQEntry;

void swap_symbol(CSymbolTableEntry* symbol) {
    symbol->checksum = READ_UINT32(symbol->checksum);
    symbol->sourceFileNameChecksum = READ_UINT32(symbol->sourceFileNameChecksum);
    symbol->union_type = READ_UINT32(symbol->union_type);
    symbol->next = READ_UINT32(symbol->next);

    CSymbolQEntry* qentry = (CSymbolQEntry*)symbol;
    switch (symbol->type) {
    case ESYMBOLTYPE_QSCRIPT:
        qentry->checksum = READ_UINT32(qentry->checksum);
        qentry->uncompressed_size = READ_UINT32(qentry->uncompressed_size);
        qentry->compressed_size = READ_UINT32(qentry->compressed_size);
        break;
    }
}
void print_symbol(CSymbolTableEntry* symbol) {
    printf("unk2: %02x\n", symbol->unk[1]);
    printf("type: %02x\n", symbol->type);
    printf("checksum: %08x\n", symbol->checksum);
    printf("sourceFileNameChecksum: %08x\n", symbol->sourceFileNameChecksum);
    printf("union_type: %08x\n", symbol->union_type);
    printf("next: %08x\n", symbol->next);

}
void print_symbol_alignment() {
    CSymbolTableEntry* v = nullptr;
    CSymbolQEntry* qentry = (CSymbolQEntry*)v;
    printf("next: %p\n", &v->next);
    return;
}

CSymbolTableEntry* AdjustQBPointers(CSymbolTableEntry* entry, uint8_t* root) {
    CSymbolTableEntry* next = entry+1;
    CSymbolQEntry* qentry = (CSymbolQEntry*)entry;
    printf("size: %d\n", sizeof(CSymbolQEntry));
    switch (entry->type) {
    case ESYMBOLTYPE_STRING:
        assert(false);
        break;
    case ESYMBOLTYPE_LOCALSTRING:
        assert(false);
        break;
    case ESYMBOLTYPE_PAIR:
        assert(false);
        break;
    case ESYMBOLTYPE_VECTOR:
        assert(false);
    break;
    case ESYMBOLTYPE_QSCRIPT:
        next = (CSymbolTableEntry*)((((((ptrdiff_t)qentry-(ptrdiff_t)root)+sizeof(CSymbolQEntry)+qentry->compressed_size)+3 ) & 0xFFFFFFFC) + root);
        return next;
        break;
    default:
        printf("offset: %08x\n", (ptrdiff_t)next - (ptrdiff_t)root);
        return next;
        break;
    }
}
void dump_qscript(CSymbolQEntry* entry, uint8_t* root) {
    ptrdiff_t offset = (ptrdiff_t)&entry->compressed_size - (ptrdiff_t)root;
    printf("QScript at %d / %p\n", offset, offset);
    show_dump((unsigned char*) & entry->compressed_size, sizeof(uint32_t) * 4, stdout);

    printf("comp size: %d / %p\n", entry->compressed_size, entry->compressed_size);
}
int main() {
    print_symbol_alignment();
    FILE *fd = fopen("D:\\thpg\\SCRIPTS\\ENGINE\\ENGINE_PARAMS.QB.PS3", "rb");
    
    fseek(fd, 0, SEEK_END);
    int file_size = ftell(fd);
    uint8_t* file_buff = new uint8_t[file_size];
    
    fseek(fd, 0, SEEK_SET);

    fread(file_buff, sizeof(uint8_t), file_size, fd);

    ScriptHeader* header = (ScriptHeader*)file_buff;
    printf("header size: %d\n", sizeof(ScriptHeader));

    uint32_t total_len = READ_UINT32(header->total_length);
    assert(total_len == file_size);
    uint8_t* file_end = file_buff + total_len;
    
    fclose(fd);

    CSymbolTableEntry* current_symbol = (CSymbolTableEntry*)&file_buff[sizeof(ScriptHeader)];
    int x = 0;
    while (current_symbol != (CSymbolTableEntry*)file_end) {
        printf("on symbol: %08x - %d\n", current_symbol->checksum, current_symbol->type);
        swap_symbol(current_symbol);
        if (current_symbol->type == ESYMBOLTYPE_QSCRIPT) {
            dump_qscript((CSymbolQEntry*)current_symbol, file_buff);
        }
        if (x++ == 5) {
            break;
        }
        current_symbol = AdjustQBPointers(current_symbol, file_buff);
    }

    return 0;
}