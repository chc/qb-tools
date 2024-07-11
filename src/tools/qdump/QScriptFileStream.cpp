#include "QScriptFileStream.h"
#include "token/QScriptToken.h"
#include <stdint.h>
#include <cassert>
#include <string.h>
QScriptFileStream::QScriptFileStream(const char *path) {
    mp_fd = fopen(path,"rb");
}
QScriptFileStream::~QScriptFileStream() {
    fclose(mp_fd);
}
bool QScriptFileStream::IsFileOpened() {
    return mp_fd != NULL;
}
QScriptToken *QScriptFileStream::NextToken() {
    if(feof(mp_fd)) {
        return NULL;
    }
    uint8_t type;
    int len = fread(&type, sizeof(uint8_t), 1, mp_fd);
    assert(len == 1);

    QScriptToken *result = nullptr;
    result = QScriptToken::Resolve(type);
    if(result) {
        result->LoadParams(this);
    }
    return result;
}
uint16_t QScriptFileStream::ReadUInt16() {
    uint16_t v;
    int len = fread(&v, sizeof(uint16_t), 1, mp_fd);

    assert(len == 1);

    return v;
}
int16_t QScriptFileStream::ReadInt16() {
    int16_t v;
    int len = fread(&v, sizeof(int16_t), 1, mp_fd);

    assert(len == 1);

    return v;
}
uint32_t QScriptFileStream::ReadUInt32() {
    uint32_t v;
    int len = fread(&v, sizeof(uint32_t), 1, mp_fd);

    assert(len == 1);

    return v;
}
int32_t QScriptFileStream::ReadInt32() {
    int32_t v;
    int len = fread(&v, sizeof(int32_t), 1, mp_fd);

    assert(len == 1);

    return v;
}
float QScriptFileStream::ReadFloat() {
    float v;
    int len = fread(&v, sizeof(float), 1, mp_fd);

    assert(len == 1);

    return v;
}
uint8_t QScriptFileStream::ReadByte() {
    uint8_t v;
    int len = fread(&v, sizeof(uint8_t), 1, mp_fd);

    assert(len == 1);

    return v;
}
const char *QScriptFileStream::ReadNTS() {
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
size_t QScriptFileStream::GetOffset() {
    return ftell(mp_fd);
}