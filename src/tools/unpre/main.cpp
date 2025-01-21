#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <FileStream.h>
#include <map>
#include <vector>

#include <string.h>
#include <pre.h>

#include <sys/stat.h>
#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

void create_dir(char *path) {
    char *p = (char *)path;
    char* x = p;
    while (*x) {
        if (*x == '\\') {
            *x = '/';
        }
        x++;
    }

    while(true) {
        char *x = strchr(p, '/');
        if(x == NULL) {
            break;
        }
        *x = 0;
#ifdef _WINDOWS

        CreateDirectoryA(path, NULL);
#else
        mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
        p = x+1;
        *x = '/';       
    }
}
void print_pre_item(PreItem* item) {

    const char* c = NULL;
    printf("extracting: %s\n", item->filename);

    uint8_t *buff = (uint8_t*)malloc(item->original_size);
    unpre_read_file(item, buff);

    create_dir(item->filename);

    FILE* out = fopen(item->filename, "wb");
    if (out) {
        fwrite(buff, item->original_size, 1, out);
        fclose(out);
    } else {
        fprintf(stderr, "Failed to open for writing: %s\n", item->filename);
    }

    delete(buff);

}


bool unpre_file_info_callback(PreItem item) {
    print_pre_item(&item);
    return true;
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s [pre_path]\n", argv[0]);
        return -1;
    }


    printf("Extracting pre at: %s\n", argv[1]);

    unpre_iterate_files(argv[1], unpre_file_info_callback);
    return 0;
}