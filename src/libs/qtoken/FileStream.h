#ifndef _FILESTREAM_H
#define _FILESTREAM_H
#include "IStream.h"
#include <stdio.h>

class FileStream : public IStream {
    public:
        FileStream(const char *path);
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
        void Align();
    private:
        FILE *mp_fd;
};
#endif //_IQFILESTREAM_H