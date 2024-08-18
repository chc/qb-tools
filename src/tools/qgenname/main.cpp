#include <stdio.h>
#include <string.h>
#include <crc32.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, const char* argv[]) {
    if (argc < 1) {
        fprintf(stderr, "usage: %s [name]\n", argv[0]);
        return -1;
    }

    char *name = strdup(argv[1]);
    int len = strlen(name);

    uint32_t checksum = crc32(0, name, len);
    printf("Checksum (case sensitive): 0x%08x - %d\n", checksum, checksum);


    
    for(int i=0;i<len;i++) {
        name[i] = tolower(name[i]);
    }

    checksum = crc32(0, name, len);
    printf("Checksum (Lower): 0x%08x - %d\n", checksum, checksum);

    free((void *)name);
    return checksum;
}