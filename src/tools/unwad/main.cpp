#include <wad.h>
#include <cassert>

#include <string.h>
#include <sys/stat.h>
#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif


FILE *list_out_fd = nullptr;
const char **file_list;
int num_files;


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

bool unwad_extract_entry_callback(WADItem *item) {
    fprintf(stderr, "File entry:\n");
    fprintf(stderr, "\tfile: %s\n", item->filename);
    fprintf(stderr, "\toffset: %08x\n", item->offset);
    fprintf(stderr, "\tsize: %d\n\n", item->size);


    uint8_t* buf = new uint8_t[item->size];
    unwad_read_file(item, buf);

    char *path = item->filename;
    if(path[0] == '\\') {
        path++;
    }

    create_dir(path);

    FILE *fd = fopen(path, "wb");
    int len = fwrite(buf, item->size, 1, fd);
    assert(len == 1);

    fclose(fd);

    delete[] buf;
    
    return true;
}


bool unwad_list_callback(WADItem *item) {
    fprintf(stderr, "File entry:\n");
    fprintf(stderr, "\tfile: %s\n", item->filename);
    fprintf(stderr, "\toffset: %08x\n", item->offset);
    fprintf(stderr, "\tsize: %d\n\n", item->size);

    if(list_out_fd != nullptr) {
        fprintf(list_out_fd, "%s\n", item->filename);
    }

    return true;
}

bool unwad_extract_file_callback(WADItem *item) {
    for(int i=0;i<num_files;i++) {
        if(strcmp(item->filename, file_list[i]) == 0) {
            unwad_extract_entry_callback(item);
            return true;
        }
    }
    return true;
}
void dump_usage(int argc, const char* argv[]) {
    fprintf(stderr, "usage: %s [hed_path] [wad_path] [mode] (options) \n", argv[0]);
    fprintf(stderr,"modes:\n\tl\tlist all entries - optional filename option to write list to file\n\te\textract specific files\n\tx\textract all entries\n");
}
int main(int argc, const char* argv[]) {
    if (argc < 4) {
        dump_usage(argc, argv);
        return -1;
    }

    WADFileInfoCallback callback = nullptr;

    const int OPTIONS_INDEX = 4;

    switch(argv[3][0]) {
        case 'l':
            callback = unwad_list_callback;
            if(argc >= 5) { //save file list to path
                list_out_fd = fopen(argv[OPTIONS_INDEX], "w");
            }


        break;
        case 'e':
            callback = unwad_extract_file_callback;
            if(argc < 5) {
                fprintf(stderr, "invalid usage for extract mode\n%s [hed_path] [wad_path] e file1 file2 file3...\n", argv[0]);
                return -1;
            }

            file_list = &argv[OPTIONS_INDEX];
            num_files = argc - OPTIONS_INDEX;
        break;
        case 'x':
            callback = unwad_extract_entry_callback;
        break;
        default:
            fprintf(stderr, "Invalid option!\n");
            dump_usage(argc, argv);
            return -1;
    }

    wad_iterate_files(argv[1], argv[2], callback);

    if(list_out_fd != nullptr) {
        fclose(list_out_fd);
    }
    return 0;
}