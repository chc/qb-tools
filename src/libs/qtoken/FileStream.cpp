#include "FileStream.h"
#include <stdint.h>
#include <cassert>
#include <string.h>
FileStream::FileStream(const char *path) {
    mp_fd = fopen(path,"rb");
}
FileStream::~FileStream() {
    fclose(mp_fd);
}
bool FileStream::IsFileOpened() {
    return mp_fd != NULL;
}
uint16_t FileStream::ReadUInt16() {
    uint16_t v;
    int len = fread(&v, sizeof(uint16_t), 1, mp_fd);

    assert(len == 1);

    return v;
}
int16_t FileStream::ReadInt16() {
    int16_t v;
    int len = fread(&v, sizeof(int16_t), 1, mp_fd);

    assert(len == 1);

    return v;
}
uint32_t FileStream::ReadUInt32() {
    uint32_t v;
    int len = fread(&v, sizeof(uint32_t), 1, mp_fd);

    assert(len == 1);

    return v;
}
int32_t FileStream::ReadInt32() {
    int32_t v;
    int len = fread(&v, sizeof(int32_t), 1, mp_fd);

    assert(len == 1);

    return v;
}
float FileStream::ReadFloat() {
    float v;
    int len = fread(&v, sizeof(float), 1, mp_fd);

    assert(len == 1);

    return v;
}
uint8_t FileStream::ReadByte() {
    uint8_t v;
    int len = fread(&v, sizeof(uint8_t), 1, mp_fd);

    assert(len == 1);

    return v;
}
const char *FileStream::ReadNTS() {
    const int MAX_STRING_LEN = 256;
    char str_buf[MAX_STRING_LEN];
    char *p = (char *)&str_buf;

    int idx = 0;
    while(idx < MAX_STRING_LEN) {
        uint8_t b;
        int len = fread(&b, sizeof(uint8_t), 1, mp_fd);
        assert(len == 1);

        *p++ = b;
        idx++;

        if(b == 0) {
            break;
        }
    }
    return strdup(str_buf);
}
size_t FileStream::GetOffset() {
    return ftell(mp_fd);
}