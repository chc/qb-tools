#ifndef _IQFILESTREAM_H
#define _IQFILESTREAM_H
#include "IQStream.h"
#include <stdio.h>

class QScriptFileStream : public IQStream {
    public:
        QScriptFileStream(const char *path);
        ~QScriptFileStream();
        QScriptToken *NextToken();
        uint16_t ReadUInt16();
        int16_t ReadInt16();
        uint32_t ReadUInt32();
        int32_t ReadInt32();
        float   ReadFloat();
        const char *ReadNTS();
        uint8_t ReadByte();
        size_t GetOffset();
        bool IsFileOpened();
    private:
        FILE *mp_fd;
};
#endif //_IQFILESTREAM_H