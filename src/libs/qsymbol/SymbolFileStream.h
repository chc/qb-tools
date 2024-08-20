#ifndef _SYMBOLFILESTREAM_H
#define _SYMBOLFILESTREAM_H

#include <FileStream.h>
#include <stdio.h>
#include <IStream.h>
#include <QSymbolToken.h>
#include <ISStream.h>

class SymbolFileStream : public ISStream {
    public:
        SymbolFileStream(IStream *data_stream);
        ~SymbolFileStream();
        QSymbol *NextSymbol();
        void WriteHeader();
        void UpdateHeaderSize();
        void WriteSymbol(QSymbol *symbol);
        void SetSourceChecksum(uint32_t checksum) { m_source_checksum = checksum;}
    private:
        IStream *mp_stream;
        uint32_t m_source_checksum;
};

#endif //_SYMBOLFILESTREAM_H