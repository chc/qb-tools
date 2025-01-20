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

static int idx = 0;
void print_pre_item(PreItem* item) {


    const char* c = NULL;
    printf("**** BEGIN ITEM (%d) ****\n", idx++);
    printf("name: %s\n", item->filename);

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
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s (options) [pre_path]\n", argv[0]);
        fprintf(stderr, "options: \n");
        fprintf(stderr, "\t-alignhack - Enable alignment logic required to extract PREs built with old modding tools\n");
        return -1;
    }

    bool alignment_hack = false;
    int arg_index = 1;
    for (int i = 1; i < argc; i++) {
        if (strstr(argv[i], "-alignhack")) {
            alignment_hack = true;
            arg_index = i+1;
        }
    }
    

    if (arg_index >= argc) {
        fprintf(stderr, "missing expected file params, run without params to see usage!\n");
        return -1;
    }

    printf("Extracting pre at: %s\n", argv[arg_index]);

    unpre_iterate_files(argv[arg_index], unpre_file_info_callback, alignment_hack);
    return 0;
}