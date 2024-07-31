#ifndef _ISTREAM_H
#define _ISTREAM_H
#include <stdint.h>
#include <stddef.h>
#define ISTREAM_LITTLE_ENDIAN 0
#define ISTREAM_BIG_ENDIAN 1
class IStream {
    public:
        //IQStream();
        //virtual ~IQStream();
        virtual uint8_t ReadByte() = 0;
        virtual uint16_t ReadUInt16() = 0;
        virtual int16_t ReadInt16() = 0;
        virtual uint32_t ReadUInt32() = 0;
        virtual int32_t ReadInt32() = 0;
        virtual float ReadFloat() = 0;
        virtual const char *ReadNTS() = 0; //This returns a *new* string pointer
        virtual size_t GetOffset() = 0;
        virtual void Seek(int32_t offset) = 0;
        virtual void SetCursor(int32_t offset) = 0;
        virtual void Align() = 0;


        virtual void WriteByte(uint8_t) = 0;
        virtual void WriteUInt16(uint16_t) = 0;
        virtual void WriteUInt32(uint32_t) = 0;
        virtual void WriteInt16(int16_t) = 0;
        virtual void WriteInt32(int32_t) = 0;
        virtual void WriteFloat(float v) = 0;
        virtual void WriteNTS(const char *v) = 0;
        virtual void WriteBuffer(uint8_t *v, uint32_t len) = 0;
        
        virtual void SetReadEndian(uint8_t endian_mode) = 0;
        virtual void SetWriteEndian(uint8_t endian_mode) = 0;
        virtual uint8_t GetReadEndianMode() = 0;
        virtual uint8_t GetWriteEndianMode() = 0;
};
#endif // _ISTREAM_H