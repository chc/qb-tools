#ifndef _SSTREAM_H
#define _SSTREAM_H

#include <FileStream.h>
#include <stdio.h>
#include <IStream.h>
#include "symbol/QSymbolToken.h"
#include "symbol/ISStream.h"

class SStream : public ISStream {
    public:
        SStream(IStream *data_stream);
        ~SStream();
        QSymbolToken *NextSymbol();
    private:
        IStream *mp_stream;
};

#endif //_SSTREAM_H