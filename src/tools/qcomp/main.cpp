#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <FileStream.h>
#include <crc32.h>
#include "qcomp.h"

#include <NameToken.h>
#include <IntegerToken.h>
#include <FloatToken.h>
#include <EqualsToken.h>
#include <ChecksumNameToken.h>

std::map<uint32_t, const char *> m_checksum_names;

uint32_t gen_checksum(std::string str) {
    char *name = strdup(str.c_str());
    int len = strlen(name);

    for(int i=0;i<len;i++) {
        name[i] = tolower(name[i]);
    }

    uint32_t checksum = crc32(0, name, len);

    m_checksum_names[checksum] = name;
    //free(name);
    return checksum;
}

bool string_is_numeric(std::string &s) {
    if (s.empty()) {
        return false;
    }
    std::string::iterator it = s.begin();
    while(it != s.end()) {
        if(!isnumber(*it)) {
            return false;
        }
        it++;
    }
    return true;
}
bool string_is_float(std::string &s) {
    if (s.empty()) {
        return false;
    }
    std::string::iterator it = s.begin();
    while(it != s.end()) {
        char ch = *it;
        if(!isnumber(ch) && ch != '.') {
            return false;
        }
        it++;
    }
    return true;
}
void emit_token(std::string &current_token, FileStream &fs_out) {
    if(string_is_numeric(current_token)) {
        int32_t v = (int32_t)strtol(current_token.c_str(), NULL, 10);
        IntegerToken it(v);
        it.Write(&fs_out);
    } else if(string_is_float(current_token)) {
        float f = atof(current_token.c_str());
        FloatToken ft(f);
        ft.Write(&fs_out);
    } else {
        printf("emit name token: %s\n", current_token.c_str());
        uint32_t checksum = gen_checksum(current_token);
        NameToken nt(checksum);
        nt.Write(&fs_out);
    }
    
}
void emit_token(int type, FileStream &fs_out) {
    printf("token token of type: %d\n", type);
    switch(type) {
        case ESCRIPTTOKEN_EQUALS:
            EqualsToken et;
            et.Write(&fs_out);
        break;
    }
}
int main(int argc, const char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s [in] [out]\n", argv[0]);
        return -1;
    }

    FILE *mp_input_fd = fopen(argv[1], "r");

    if(!mp_input_fd) {
        fprintf(stderr, "Failed to open input file: %s\n", argv[1]);
        return -1;
    }

    FileStream fsout(argv[2], true);

    if(!fsout.IsFileOpened()) {
        fprintf(stderr, "Failed to open file: %s\n", argv[2]);
        return -1;
    }

    std::string current_token;
    while(true) {
        int ch = fgetc(mp_input_fd);
        if(ch == EOF) {
            break;
        }
        int emit_type = 0;
        switch(ch) {
            case '=':
                emit_type = ESCRIPTTOKEN_EQUALS;
            break;
            case ' ':
                continue;
            break;
        }
        if(emit_type != 0) {
            emit_token(current_token, fsout);
            current_token.clear();
            emit_token(emit_type, fsout);
            emit_type = 0;
            continue;
        }

        current_token += ch;
    }
    emit_token(current_token, fsout);


    std::map<uint32_t, const char *>::iterator it = m_checksum_names.begin();
    while(it != m_checksum_names.end()) {
        std::pair<uint32_t, const char*> p = *it;
        it++;
        if(p.first == 0 || p.second == NULL)
            continue;
        ChecksumNameToken token(p.first, p.second);
        token.Write(&fsout);
    }
    fsout.WriteByte(ESCRIPTTOKEN_ENDOFFILE);
    return 0;
}