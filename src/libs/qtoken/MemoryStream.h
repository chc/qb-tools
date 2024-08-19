#ifndef _MEMSTREAM_H
#define _MEMSTREAM_H
#include "IStream.h"
#include <stdio.h>

class MemoryStream : public IStream {
    public:
        MemoryStream(void *address, size_t len);
        ~MemoryStream();
        uint16_t ReadUInt16();
        int16_t ReadInt16();
        uint32_t ReadUInt32();
        int32_t ReadInt32();
        float   ReadFloat();
        const char *ReadNTS();
        uint8_t ReadByte();
        size_t GetOffset();
        bool IsFileOpened();
        void Seek(int32_t offset);
        void SetCursor(int32_t offset);
        void Align();
        void WriteAlign(int alignment = 4);

        void WriteByte(uint8_t);
        void WriteUInt16(uint16_t);
        void WriteUInt32(uint32_t);
        void WriteInt16(int16_t);
        void WriteInt32(int32_t);
        void WriteFloat(float v);
        void WriteNTS(const char *v);
        void WriteBuffer(uint8_t *v, uint32_t len);

        void SetReadEndian(uint8_t endian_mode);
        void SetWriteEndian(uint8_t endian_mode);
        uint8_t GetReadEndianMode() { return m_read_endian_mode; }
        uint8_t GetWriteEndianMode() { return m_write_endian_mode; }
    private:
        void *mp_head;
        void *mp_cursor;
        void *mp_end;
        size_t m_total_len;
        uint8_t m_read_endian_mode;
        uint8_t m_write_endian_mode;        
};
#endif //_MEMSTREAM_H