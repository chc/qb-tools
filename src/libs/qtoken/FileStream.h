#ifndef _FILESTREAM_H
#define _FILESTREAM_H
#include "IStream.h"
#include <stdio.h>

class FileStream : public IStream {
    public:
        FileStream(const char *path, bool create_new = false);
        ~FileStream();
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
        void Align(); //this is read align
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
        FILE* GetHandle() { return mp_fd; }
    private:
        FILE *mp_fd;
        uint8_t m_read_endian_mode;
        uint8_t m_write_endian_mode;
};

//this is used for stuff like pregen and wadgen where THPS expects backslash for file paths which breaks on unix
FILE *thps_fopen(const char *path, const char *mode);
#endif //_IQFILESTREAM_H