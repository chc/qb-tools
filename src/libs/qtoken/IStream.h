#ifndef _ISTREAM_H
#define _ISTREAM_H
#include <stdint.h>
#include <stddef.h>
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
};
#endif // _ISTREAM_H