#include "FileStream.h"
#include <stdint.h>
#include <cassert>
#include <string.h>

#define _BYTE_SWAP

#if defined(_BYTE_SWAP)
    #define READ_UINT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))
    #define READ_UINT16(x) ((x>>8) | (x<<8))
    #define READ_INT32(x) ((int32_t)READ_UINT32((uint32_t)x))
    #define READ_INT16(x) ((int16_t)READ_UINT16((uint16_t)x))
#else
    #define READ_UINT32(x) x
    #define READ_UINT16(x) x
    #define READ_INT32(x)  x
#endif

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

    return READ_UINT16(v);
}
int16_t FileStream::ReadInt16() {
    int16_t v;
    int len = fread(&v, sizeof(int16_t), 1, mp_fd);

    assert(len == 1);

    return READ_INT16(v);
}
uint32_t FileStream::ReadUInt32() {
    uint32_t v;
    int len = fread(&v, sizeof(uint32_t), 1, mp_fd);

    assert(len == 1);
    return READ_UINT32(v);
}
int32_t FileStream::ReadInt32() {
    int32_t v;
    int len = fread(&v, sizeof(int32_t), 1, mp_fd);

    assert(len == 1);

    return READ_INT32(v);
}
float FileStream::ReadFloat() {
    #ifdef _BYTE_SWAP
    uint32_t v = ReadUInt32();
    float *x = (float *)&v;
    return *x;
    #else
    float v;
    int len = fread(&v, sizeof(float), 1, mp_fd);
    assert(len == 1);
    return v;
    #endif
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
void FileStream::Seek(int32_t offset) {
    fseek(mp_fd, offset, SEEK_CUR);
}
void FileStream::SetCursor(int32_t offset) {
    fseek(mp_fd, offset, SEEK_SET);
}