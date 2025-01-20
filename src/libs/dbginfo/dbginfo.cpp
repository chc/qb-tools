#include "dbginfo.h"
#include <stdint.h>
#include <pak.h>
#include <stdio.h>
#include <cassert>
#include <string.h>
#include <map>



extern "C" {
    std::map<uint32_t, const char*> loaded_checksums;
    std::map<uint32_t, uint32_t> loaded_offsets;
}

FILE *mp_read_dbginfo_head = nullptr;
FILE *mp_read_dbginfo_body = nullptr;

bool dbginfo_load_cache(const char* path) {
    char temp_path[256];
    snprintf(temp_path, sizeof(temp_path), "%s.head", path);
    mp_read_dbginfo_head = fopen(temp_path, "rb");
    if(!mp_read_dbginfo_head) {
        printf("failed to open dbginfo head\n");
        return false;
    }

    snprintf(temp_path, sizeof(temp_path), "%s.body", path);
    mp_read_dbginfo_body = fopen(temp_path, "rb");
    if(!mp_read_dbginfo_body) {
        fclose(mp_read_dbginfo_head);
        printf("failed to open dbginfo body\n");
        return false;
    }

    return true;
}

bool dbginfo_get_checksum_offset(uint32_t search_checksum, uint64_t &offset, FILE *head) {
    if(loaded_offsets.find(search_checksum) != loaded_offsets.end()) {
        offset = loaded_offsets[search_checksum];
        return true;
    }

    if(!head) {
        return false;
    }

    fseek(head, 0, SEEK_SET);

    do {
        uint32_t checksum;
        int c = fread(&checksum, sizeof(uint32_t), 1, head);
        if(c != 1) break;
        if(checksum == search_checksum) {
            uint64_t o;
            c = fread(&o, sizeof(uint64_t), 1, head);
            if(c != 1) break;
            offset = o;
            loaded_offsets[search_checksum] = offset;
            return true;
        } else {
            if(fseek(head, sizeof(uint64_t), SEEK_CUR)) {
                break;
            }
        }
    } while(true);

    return false;
}
bool dbginfo_append_cache(const char* path, DbgChecksumInfo *info) {

    char temp_path[256];
    snprintf(temp_path, sizeof(temp_path), "%s.head", path);
    FILE *head = fopen(temp_path, "ab+");
    if(!head) {
        printf("failed to open dbginfo head for writing\n");
        return false;
    }

    uint64_t offset;
    if(dbginfo_get_checksum_offset(info->checksum, offset, head)) {
        printf("*** already added\n");
        fclose(head);
        return false;
    }

    snprintf(temp_path, sizeof(temp_path), "%s.body", path);
    FILE *body = fopen(temp_path, "ab+");
    if(!body) {
        fclose(head);
        printf("failed to open dbginfo body for writing\n");
        return false;
    }

    fseek(body, 0, SEEK_END);
    offset = ftell(body);
    fwrite(info->name, 1, strlen(info->name) + 1, body);
    
    fseek(head,0,SEEK_END);
    fwrite(&info->checksum, 1, sizeof(uint32_t), head);
    fwrite(&offset, 1, sizeof(uint64_t), head);

    fclose(head);
    fclose(body);
    
    return true;
}

const char *dbginfo_resolve(uint32_t checksum) {
    if(loaded_checksums.find(checksum) != loaded_checksums.end()) {
        return loaded_checksums[checksum];
    }

    uint64_t offset;
    char temp_buffer[256];
    int idx = 0;
    if(dbginfo_get_checksum_offset(checksum, offset, mp_read_dbginfo_head)) {
        fseek(mp_read_dbginfo_body, offset, SEEK_SET);
        do {
            
            uint8_t c;
            int len = fread(&c, sizeof(uint8_t), 1, mp_read_dbginfo_body);
            if(idx > sizeof(temp_buffer)) {
                return nullptr;
            }
            if(len == 0) break;
            temp_buffer[idx++] = c;
            if(c == 0) break;
        } while(true);
    } else {
        return nullptr;
    }

    temp_buffer[idx] = 0;

    const char *result = strdup(temp_buffer);
    loaded_checksums[checksum] = result;

    return result;
}
