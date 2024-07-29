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
        QSymbolToken *NextSymbol();
    private:
        IStream *mp_stream;
};

#endif //_SYMBOLFILESTREAM_H