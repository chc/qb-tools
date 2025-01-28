#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <FileStream.h>
#include <map>
#include <vector>

#include <string.h>
#include <pre.h>

void import_file_list(PreContext *ctx, const char *file_list) {
    char line[PRE_MAX_FILENAME];
    int line_index = 0;

    FILE *fd = fopen(file_list, "r");
    if(!fd) {
        fprintf(stderr, "Failed to open file list: %s\n", file_list);
        return;
    }

    bool active = true;
    while(active) {
        int c = fgetc(fd);
        if(c == EOF) {
            break;
        }
        switch(c) {
            case '\r':
            break;
            case '\n':
                line[line_index] = 0;
                line_index = 0;
                printf("Adding file to pre: %s\n", line);
                pre_append_file(ctx, line);
            break;
            default:
                if(line_index >= sizeof(line)) {
                    line_index = 0;
                    fprintf(stderr, "line max length overflow!\n");
                    active = false;
                    break;
                }
                line[line_index++] = c;
            break;
        }
    }
    if(line_index > 0) {
        line[line_index] = 0;
        line_index = 0;
        printf("Adding file to pre: %s\n", line);
        pre_append_file(ctx, line);
    }
    fclose(fd);
}

int main(int argc, const char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "usage: %s [mode] [pre_path] [file] \n", argv[0]);
        fprintf(stderr, "mode: \n");
        fprintf(stderr, "\tc - Create a new pre file with, optionally with initial file\n");
        fprintf(stderr, "\ta - Append file to a pre file\n");
        fprintf(stderr, "\tf - Create and import file list\n");
        return -1;
    }


    PreContext *ctx = nullptr;

    bool file_mode = false;
    switch(argv[1][0]) {
        case 'c':
            ctx = pre_create(argv[2]);
            break;
        case 'a':
            ctx = pre_open(argv[2]);
            break;
        case 'f':
            ctx = pre_create(argv[2]);
            file_mode = true;
        break;
        default:
            fprintf(stderr, "unknown mode: %c\n",argv[1][0]);
            return -1;
    }

    if(file_mode) {
        if(argc <= 3) {
            fprintf(stderr, "File list path required!\n");
            return -1;
        }
        import_file_list(ctx, argv[3]);
    } else if(argc > 3) {
        printf("Adding file to pre: %s\n",  argv[3]);
        pre_append_file(ctx, argv[3]);
    }
    
    if(ctx != nullptr) {
        pre_close(ctx);
    }

    return 0;
}
