#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <FileStream.h>
#include <map>
#include <vector>

#include <string.h>
#include <pak.h>
#include <dbginfo.h>

#include <dirent.h>
#include <sys/stat.h>

void handle_directory(PakContext *ctx, const char *dir_name, int initial_len) {
    struct dirent *entry;
    DIR *dp;
    dp = opendir(dir_name);

    while(entry = readdir(dp)) {
        const int expected_len = (strlen(dir_name) + entry->d_namlen) + 2;
        char *temp_str = (char *)malloc(expected_len);
        snprintf(temp_str, expected_len, "%s/%s", dir_name, entry->d_name);

        if(entry->d_type & DT_DIR) {
            if(entry->d_name[0] != '.') {
                handle_directory(ctx, temp_str, initial_len);
                
            }                
        } else if (entry->d_type & DT_REG){
            if(entry->d_name[0] != '.') {
                printf("Adding file to pak: %s\n", temp_str + initial_len);
                pak_append_file(ctx, temp_str + initial_len); // +2 to skip "./"     
            }           
        }
        free((void *)temp_str);
        
    }
    closedir(dp);
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s [data_dir] [pak_path] (pab_path)\n", argv[0]);
        return -1;
    }


    PakContext *ctx = pak_create(argv[2], argv[3]);

    const char *data_dir = argv[1];
    handle_directory(ctx, data_dir, strlen(data_dir)+1);

    pak_close(ctx);
    
    return 0;
}