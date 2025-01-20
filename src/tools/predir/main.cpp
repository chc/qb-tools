#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <FileStream.h>
#include <map>
#include <vector>

#include <string.h>
#include <pre.h>

#ifdef _WINDOWS
#include <win_dirent.h>
#else
#include <dirent.h>
#endif
#include <sys/stat.h>

void handle_directory(PreContext *ctx, const char *dir_name, int initial_len) {
    struct dirent *entry;
    DIR *dp;
    dp = opendir(dir_name);

    if(!dp) {
        printf("failed to open dir: %s\n", dir_name);
        return;
    }

    while(entry = readdir(dp)) {
	const int expected_len = (strlen(dir_name) + strlen(entry->d_name)) + 2; // +2 for "/" and null byte
        char *temp_str = (char *)malloc(expected_len);
        snprintf(temp_str, expected_len, "%s/%s", dir_name, entry->d_name);

        if(entry->d_type & DT_DIR) {
            if(entry->d_name[0] != '.') {
                handle_directory(ctx, temp_str, initial_len);
                
            }                
        } else if (entry->d_type & DT_REG){
            if(entry->d_name[0] != '.') {
                printf("Adding file to pak: %s\n", temp_str + initial_len);
                pre_append_file(ctx, temp_str + initial_len);  
            }           
        }
        free((void *)temp_str);
        
    }
    closedir(dp);
}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s [data_dir] [pre_path]\n", argv[0]);
        return -1;
    }


    PreContext *ctx = pre_create(argv[2]);

    if(ctx == nullptr) {
        return -1;
    }

    const char *data_dir = argv[1];
    handle_directory(ctx, data_dir, strlen(data_dir)+1);

    pre_close(ctx);
    
    return 0;
}
