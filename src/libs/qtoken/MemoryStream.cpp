#include "MemoryStream.h"
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
    
MemoryStream::MemoryStream(void *address, size_t len) {
    m_read_endian_mode = ISTREAM_LITTLE_ENDIAN;
    m_write_endian_mode = ISTREAM_LITTLE_ENDIAN;

    mp_head = address;
    mp_cursor = address;
    mp_end = (void *)((ptrdiff_t)mp_head + (ptrdiff_t)len);
    
}
MemoryStream::~MemoryStream() {
}
bool MemoryStream::IsFileOpened() {
    return true;
}
uint16_t MemoryStream::ReadUInt16() {
    uint16_t v = *(uint16_t *)mp_cursor;
    mp_cursor = (void *)((ptrdiff_t)mp_cursor + sizeof(uint16_t));

    if(m_read_endian_mode == ISTREAM_BIG_ENDIAN) {
        return SWAP_UINT16(v);
    }
    return v;
}
int16_t MemoryStream::ReadInt16() {
    int16_t v = *(int16_t *)mp_cursor;
    mp_cursor = (void *)((ptrdiff_t)mp_cursor + sizeof(int16_t));

    if(m_read_endian_mode == ISTREAM_BIG_ENDIAN) {
        return SWAP_INT16(v);
    }
    return v;
}
uint32_t MemoryStream::ReadUInt32() {
    uint32_t v = *(uint32_t *)mp_cursor;
    mp_cursor = (void *)((ptrdiff_t)mp_cursor + sizeof(uint32_t));

    if(m_read_endian_mode == ISTREAM_BIG_ENDIAN) {
        return SWAP_UINT32(v);
    }
    return v;
}
int32_t MemoryStream::ReadInt32() {
    int32_t v = *(int32_t *)mp_cursor;
    mp_cursor = (void *)((ptrdiff_t)mp_cursor + sizeof(int32_t));

    if(m_read_endian_mode == ISTREAM_BIG_ENDIAN) {
        return SWAP_INT32(v);
    }
    return v;
}
float MemoryStream::ReadFloat() {
    float v = *(float *)mp_cursor;
    mp_cursor = (void *)((ptrdiff_t)mp_cursor + sizeof(float));

    if(m_read_endian_mode == ISTREAM_BIG_ENDIAN) {
        SWAP_FLOAT(v)
    }
    return v;
}
uint8_t MemoryStream::ReadByte() {
    uint8_t v = *(uint8_t *)mp_cursor;
    mp_cursor = (void *)((ptrdiff_t)mp_cursor + sizeof(uint8_t));

    return v;
}
const char *MemoryStream::ReadNTS() {
    const int MAX_STRING_LEN = 256;
    char str_buf[MAX_STRING_LEN];
    char *p = (char *)&str_buf;

    int idx = 0;
    while(idx < MAX_STRING_LEN) {
        uint8_t b = ReadByte();

        *p++ = b;
        idx++;

        if(b == 0) {
            break;
        }
    }
    return strdup(str_buf);
}
size_t MemoryStream::GetOffset() {
    return (ptrdiff_t)mp_cursor - (ptrdiff_t)mp_head;
}
void MemoryStream::Seek(int32_t offset) {

}
void MemoryStream::SetCursor(int32_t offset) {
    void *new_addr = (void*)((ptrdiff_t)mp_head + (ptrdiff_t)offset);
    if(new_addr > mp_end) {
        return;
    }
    mp_cursor = new_addr;

}
void MemoryStream::Align() {
    while((GetOffset() % 4)) {
        ReadByte();
    }
}

void MemoryStream::WriteAlign(int alignment) {
    while((GetOffset() % 4)) {
        WriteByte(0);
    }
}

void MemoryStream::WriteByte(uint8_t v) {
    uint8_t *end = (uint8_t*)((ptrdiff_t)mp_cursor + sizeof(uint8_t));
    if(end > mp_end) {
        assert(false);
        return;
    }
    *(uint8_t*)mp_cursor = v;
    mp_cursor = end;
}

void MemoryStream::WriteUInt16(uint16_t v) {
    uint16_t *end = (uint16_t*)((ptrdiff_t)mp_cursor + sizeof(uint16_t));
    if(end > mp_end) {
        assert(false);
        return;
    }

    if(m_write_endian_mode == ISTREAM_BIG_ENDIAN) {
        v = SWAP_UINT16(v);
    }
    *(uint16_t*)mp_cursor = v;
    mp_cursor = end;
}

void MemoryStream::WriteUInt32(uint32_t v) {
    uint32_t *end = (uint32_t*)((ptrdiff_t)mp_cursor + sizeof(uint32_t));
    if(end > mp_end) {
        assert(false);
        return;
    }

    if(m_write_endian_mode == ISTREAM_BIG_ENDIAN) {
        v = SWAP_UINT32(v);
    }
    *(uint32_t*)mp_cursor = v;
    mp_cursor = end;
}

void MemoryStream::WriteInt16(int16_t v) {
    int16_t *end = (int16_t*)((ptrdiff_t)mp_cursor + sizeof(int16_t));
    if(end > mp_end) {
        assert(false);
        return;
    }

    if(m_write_endian_mode == ISTREAM_BIG_ENDIAN) {
        v = SWAP_INT16(v);
    }
    *(int16_t*)mp_cursor = v;
    mp_cursor = end;
}

void MemoryStream::WriteInt32(int32_t v) {
    int32_t *end = (int32_t*)((ptrdiff_t)mp_cursor + sizeof(int32_t));
    if(end > mp_end) {
        assert(false);
        return;
    }

    if(m_write_endian_mode == ISTREAM_BIG_ENDIAN) {
        v = SWAP_INT32(v);
    }
    *(int32_t*)mp_cursor = v;
    mp_cursor = end;
}
void MemoryStream::WriteFloat(float v) {
    float *end = (float*)((ptrdiff_t)mp_cursor + sizeof(float));
    if(end > mp_end) {
        assert(false);
        return;
    }
    if(m_write_endian_mode == ISTREAM_BIG_ENDIAN) {
        SWAP_FLOAT(v)
    }
    *(float*)mp_cursor = v;
    mp_cursor = end;
}

void MemoryStream::WriteNTS(const char *v) {
    WriteBuffer((uint8_t*)v, strlen(v)+1);
}
void MemoryStream::WriteBuffer(uint8_t *v, uint32_t len) {
    void *end = (int32_t*)((ptrdiff_t)mp_cursor + len);
    if(end > mp_end) {
        assert(false);
        return;
    }
    memcpy(mp_cursor, v, len);
    mp_cursor = end;
}
void MemoryStream::SetReadEndian(uint8_t endian_mode) {
    m_read_endian_mode = endian_mode;
}
void MemoryStream::SetWriteEndian(uint8_t endian_mode) {
    m_write_endian_mode = endian_mode;
}