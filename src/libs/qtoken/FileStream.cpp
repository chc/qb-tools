#include "FileStream.h"
#include <stdint.h>
#include <cassert>
#include <string.h>

//this is only designed to run on little endian cpus - so swap is to/from big endian
#define SWAP_UINT32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) | (((x) & 0x0000FF00) << 8) | ((x) << 24))
#define SWAP_UINT16(x) ((x>>8) | (x<<8))
#define SWAP_INT32(x) ((int32_t)SWAP_UINT32((uint32_t)x))
#define SWAP_INT16(x) ((int16_t)SWAP_UINT16((uint16_t)x))
#define SWAP_FLOAT(x) { \
        uint32_t *vir = (uint32_t *)&x; \
        *vir = SWAP_UINT32(*vir); \
        float *vr = (float *)vir; \
        v = *vr; \
    }
    
FileStream::FileStream(const char *path, bool is_write) {
    m_read_endian_mode = ISTREAM_LITTLE_ENDIAN;
    m_write_endian_mode = ISTREAM_LITTLE_ENDIAN;
    if(is_write) {
        mp_fd = fopen(path,"wb"); 
    } else {
        mp_fd = fopen(path,"rb");
    }
    assert(mp_fd);
    
}
FileStream::~FileStream() {
    assert(mp_fd);
    fclose(mp_fd);
}
bool FileStream::IsFileOpened() {
    return mp_fd != NULL;
}
uint16_t FileStream::ReadUInt16() {
    uint16_t v;
    int len = fread(&v, sizeof(uint16_t), 1, mp_fd);

    assert(len == 1);

    if(m_read_endian_mode == ISTREAM_BIG_ENDIAN) {
        return SWAP_UINT16(v);
    }
    return v;
}
int16_t FileStream::ReadInt16() {
    int16_t v;
    int len = fread(&v, sizeof(int16_t), 1, mp_fd);

    assert(len == 1);

    if(m_read_endian_mode == ISTREAM_BIG_ENDIAN) {
        return SWAP_INT16(v);
    }
    return v;
}
uint32_t FileStream::ReadUInt32() {
    uint32_t v;
    int len = fread(&v, sizeof(uint32_t), 1, mp_fd);

    assert(len == 1);
    if(m_read_endian_mode == ISTREAM_BIG_ENDIAN) {
        return SWAP_UINT32(v);
    }
    return v;
}
int32_t FileStream::ReadInt32() {
    int32_t v;
    int len = fread(&v, sizeof(int32_t), 1, mp_fd);

    assert(len == 1);

    if(m_read_endian_mode == ISTREAM_BIG_ENDIAN) {
        return SWAP_INT32(v);
    }
    return v;
}
float FileStream::ReadFloat() {
    float v;
    int len = fread(&v, sizeof(float), 1, mp_fd);
    assert(len == 1);

    if(m_read_endian_mode == ISTREAM_BIG_ENDIAN) {
        SWAP_FLOAT(v)
    }
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
void FileStream::Seek(int32_t offset) {
    fseek(mp_fd, offset, SEEK_CUR);
}
void FileStream::SetCursor(int32_t offset) {
    fseek(mp_fd, offset, SEEK_SET);
}
void FileStream::Align() {
    while((GetOffset() % 4)) {
        ReadByte();
    }
}
void FileStream::WriteAlign(int alignment) {
    while((GetOffset() % alignment)) {
        WriteByte(0);
    }
}

void FileStream::WriteByte(uint8_t v) {
    int len = fwrite(&v, sizeof(uint8_t), 1, mp_fd);

    assert(len == 1);
}

void FileStream::WriteUInt16(uint16_t v) {
    if(m_write_endian_mode == ISTREAM_BIG_ENDIAN) {
        v = SWAP_UINT16(v);
    }
    int len = fwrite(&v, sizeof(uint16_t), 1, mp_fd);
    assert(len == 1);
}

void FileStream::WriteUInt32(uint32_t v) {
    if(m_write_endian_mode == ISTREAM_BIG_ENDIAN) {
        v = SWAP_UINT32(v);
    }
    int len = fwrite(&v, sizeof(uint32_t), 1, mp_fd);
    assert(len == 1);
}

void FileStream::WriteInt16(int16_t v) {
    if(m_write_endian_mode == ISTREAM_BIG_ENDIAN) {
        v = SWAP_INT16(v);
    }
    int len = fwrite(&v, sizeof(int16_t), 1, mp_fd);
    assert(len == 1);
}

void FileStream::WriteInt32(int32_t v) {
    if(m_write_endian_mode == ISTREAM_BIG_ENDIAN) {
        v = SWAP_INT32(v);
    }
    int len = fwrite(&v, sizeof(int32_t), 1, mp_fd);
    assert(len == 1);
}

void FileStream::WriteFloat(float v) {
    if(m_write_endian_mode == ISTREAM_BIG_ENDIAN) {
        SWAP_FLOAT(v)
    }
    int len = fwrite(&v, sizeof(float), 1, mp_fd);
    assert(len == 1);
}

void FileStream::WriteNTS(const char *v) {
    int len = fwrite(v, strlen(v) + 1, 1, mp_fd);
    assert(len == 1);
}
void FileStream::WriteBuffer(uint8_t *v, uint32_t len) {
    int w_len = fwrite(v, len, 1, mp_fd);
    assert(w_len == 1);
}
void FileStream::SetReadEndian(uint8_t endian_mode) {
    m_read_endian_mode = endian_mode;
}
void FileStream::SetWriteEndian(uint8_t endian_mode) {
    m_write_endian_mode = endian_mode;
}