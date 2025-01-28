#include <wad.h>

void import_file_list(WADContext *ctx, const char *file_list) {
    char line[MAX_WAD_NAME];
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
                printf("Adding file to wad: %s\n", line);
                wad_append_file(ctx, line);
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
        printf("Adding file to wad: %s\n", line);
        wad_append_file(ctx, line);
    }
    fclose(fd);
}

int main(int argc, const char* argv[]) {
    if (argc < 4) {
        fprintf(stderr, "usage: %s [hed_path] [wad_path] [file_list_path] \n", argv[0]);
        return -1;
    }

    WADContext *ctx = wad_create(argv[1], argv[2]);
    
    import_file_list(ctx, argv[3]);

    wad_close(ctx);

    return 0;
}