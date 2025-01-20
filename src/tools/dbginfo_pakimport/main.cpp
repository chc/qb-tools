#include <stdio.h>
#include <string.h>
#include <crc32.h>
#include <ctype.h>
#include <stdlib.h>
#include <dbginfo.h>
#include <pak.h>

bool unpak_file_info_callback(PakItem item) {
    uint8_t* buf = new uint8_t[item.size];
    unpak_read_file(item, buf);

    delete[] buf;
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: [pak_path]\n", argv[0]);
        return -1;
    }

    unpak_iterate_files(argv[1], nullptr, unpak_file_info_callback);
    return 0;
}